#include <Kiss/pch.h>

#include <kinc/graphics4/graphics.h>
#include <kinc/graphics4/vertexbuffer.h>
#include <kinc/graphics4/vertexstructure.h>
#include <kinc/graphics4/indexbuffer.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/texture.h>

#include "imgui.h"
#include "imgui_g4.h"

#include <Kiss/render/gfx2dDefs.h>
#include <Kiss/render/bufferManager.h>

using namespace kiss;
using namespace kiss::gfx2d;

namespace {
    static kinc_g4_texture_t g_Texture;
    static bool fontInitialized = false;

    struct ImKincVert
    {
	    ImVec2  pos;
	    s16  u;
        s16  v;
	    u32 col;
    };

    static void invalidateDeviceObjects();
    static void createDeviceObjects();
    static void createFontsTexture();

    
    static inline void ImGui_ImplG4_SetupRenderState(ImDrawData* draw_data)
    {
        kinc_g4_set_pipeline(&base::pipeline.pipe);
    }

    static void invalidateDeviceObjects()
    {
        kinc_g4_texture_destroy(&g_Texture);
        fontInitialized = false;
    }

    static void createDeviceObjects()
    {
         if (fontInitialized)
            invalidateDeviceObjects();
        createFontsTexture();
    }

    static void createFontsTexture()
    {
        // Build texture atlas
        ImGuiIO& io = ImGui::GetIO();
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        {
            kinc_g4_texture_init(&g_Texture, width, height, KINC_IMAGE_FORMAT_RGBA32);
            unsigned char* tex = kinc_g4_texture_lock(&g_Texture);
            int stride = kinc_g4_texture_stride(&g_Texture);
            for (int y = 0; y < height; ++y)
            {
                memcpy(&tex[y * stride], &pixels[y * width * 4], (s64)width * 4);
            }
            kinc_g4_texture_unlock(&g_Texture);
        }
        // Store our identifier
        io.Fonts->TexID = (ImTextureID)&g_Texture;
        // Create texture sampler
        fontInitialized = true;
    }
}

void kiss::imgui::gfx::init()
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_g4";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
}

void kiss::imgui::gfx::release()
{
    invalidateDeviceObjects();
}

void kiss::imgui::gfx::prerender()
{
    if (!fontInitialized)
        createDeviceObjects();
}

void kiss::imgui::gfx::render(ImDrawData* draw_data)
{
    kinc_g4_vertex_buffer_t* g_VB = vbuffer::manager.get();
    kinc_g4_index_buffer_t* g_IB = ibuffer::manager.get();

    ImKincVert* vtx_dst = (ImKincVert*)kinc_g4_vertex_buffer_lock(g_VB, 0, draw_data->TotalVtxCount);
    auto* idx_dst = kinc_g4_index_buffer_lock(g_IB);

    int global_vtx_offset = 0;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        for (int i = 0; i < cmd_list->VtxBuffer.Size; ++i)
        {
            vtx_dst[i].pos = cmd_list->VtxBuffer.Data[i].pos;
            vtx_dst[i].u = (u16)(cmd_list->VtxBuffer.Data[i].uv.x * 32767);
            vtx_dst[i].v = (u16)(cmd_list->VtxBuffer.Data[i].uv.y * 32767);
            vtx_dst[i].col = cmd_list->VtxBuffer.Data[i].col;
        }
        for (int i = 0; i < cmd_list->IdxBuffer.Size; ++i)
        {
            idx_dst[i] = (u32)cmd_list->IdxBuffer.Data[i] + global_vtx_offset;
        }
        //TODO this cannot be done due to index buffer 32 16 implementation.
        //memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
        vtx_dst += cmd_list->VtxBuffer.Size;
        idx_dst += cmd_list->IdxBuffer.Size;
        global_vtx_offset += cmd_list->VtxBuffer.Size;
    }
    kinc_g4_vertex_buffer_unlock(g_VB, draw_data->TotalVtxCount);
    kinc_g4_index_buffer_unlock(g_IB);

    // Setup desired DX state
    ImGui_ImplG4_SetupRenderState(draw_data);

    {// Setup Projection.
        kinc_g4_set_matrix3(base::pipeline.proj_location, &xform::gui);
    }

    // Render command lists
    // (Because we merged all buffers into a single one, we maintain our own offset into them)
    int global_idx_offset = 0;
    ImVec2 clip_off = draw_data->DisplayPos;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplG4_SetupRenderState(draw_data);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // Apply scissor/clipping rectangle
                kinc_g4_scissor((int)(pcmd->ClipRect.x - clip_off.x), (int)(pcmd->ClipRect.y - clip_off.y), (int)(pcmd->ClipRect.z - clip_off.x), (int)(pcmd->ClipRect.w - clip_off.y));

                // Bind texture, Draw
                kinc_g4_set_texture(base::pipeline.texture_unit, (kinc_g4_texture*)pcmd->TextureId);
                kinc_g4_set_vertex_buffer(g_VB);
                kinc_g4_set_index_buffer(g_IB);
                kinc_g4_draw_indexed_vertices_from_to(pcmd->IdxOffset + global_idx_offset, pcmd->ElemCount);
                //kinc_g4_draw_indexed_vertices_from_to_from(pcmd->IdxOffset + global_idx_offset, pcmd->ElemCount, pcmd->VtxOffset + global_vtx_offset);
            }
        }
        global_idx_offset += cmd_list->IdxBuffer.Size;
        global_vtx_offset += cmd_list->VtxBuffer.Size;
    }
}