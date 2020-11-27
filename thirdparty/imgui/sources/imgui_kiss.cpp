#include <Kiss/pch.h>
#include <Kiss/app.h>
#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
//#include <kinc/input/surface.h>
#include <kinc/system.h>
#include <kinc/window.h>

#include "imgui.h"
#include "imgui_kiss.h"
#include "private/imgui_g4.h"
#include "private/imgui_input.h"

namespace kiss 
{
    namespace imgui
    {
        void init()
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            auto& io = ImGui::GetIO();
            io.BackendPlatformName = "kiss";

            ImGui::StyleColorsDark();

            input::init();
            gfx::init();

            /*io.SetClipboardTextFn = ImGui_ImplKinc_SetClipboardText;
            io.GetClipboardTextFn = ImGui_ImplKinc_GetClipboardText;
            io.ClipboardUserData = NULL;*/
        }

        void render(float dt) {
            auto& io = ImGui::GetIO();
            io.DeltaTime = dt > 0 ? dt : (float)(1.0f / 60.0f);
            input::update();
            gfx::prerender();
            ImGui::NewFrame();
            kiss::app::gui(dt);
            ImGui::Render();
            gfx::render(ImGui::GetDrawData());
        }

        void resize(int w, int h) {
            auto& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)w, (float)h);
        }

        void release() {
            gfx::release();
            ImGui::DestroyContext();
        }
    }
}