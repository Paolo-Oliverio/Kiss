#include "app.h"
#include "time/delta.h"
#include <Kiss/render/basicPipeline.h>
#include <kinc/graphics4/graphics.h>
#include <kinc/system.h>
#include <kinc/window.h>

//#include "sound/soundSystem.h"
#ifdef KISS_IMGUI
	#include <imgui.h>
	#include <imgui_impl_kinc.h>
	#include <imgui_impl_g4.h>
#endif

namespace kiss {

	namespace framework {

		static inline void render(float dt) 
		{
			kinc_g4_begin(0);
			app::on_render(dt);
			#ifdef KISS_IMGUI
				// Start the Dear ImGui frame
				ImGui_ImplG4_NewFrame();
				ImGui_ImplKinc_NewFrame(0);
				ImGui::NewFrame();
				app::on_imgui(dt);
				ImGui::Render();
				ImGui_ImplG4_RenderDrawData(ImGui::GetDrawData());
			#endif
			kinc_g4_end(0);
			kinc_g4_swap_buffers();
		}

		void update() 
		{
			auto dt = time.update();
			app::on_update(dt);
			render(dt);
		}

		void shutdown() 
		{
			#ifdef KISS_IMGUI
				// Cleanup
				ImGui_ImplG4_Shutdown();
				ImGui_ImplKinc_Shutdown();
				ImGui::DestroyContext();
			#endif

			app::on_shutdown();
			gfx2d::release();
		}

		void resize(int x, int y, void* data){
			gfx2d::on_resize((float)x, (float)y);
#ifdef KORE_OPENGL
			kinc_g4_restore_render_target();
#endif
			app::on_resize(x,y);
		}

		void init(const char* title) 
		{
			kinc_framebuffer_options frame;
			frame.vertical_sync = false;
			frame.depth_bits = 0;
			frame.samples_per_pixel = 1;
			frame.stencil_bits = 0;
			kinc_init(title, 1280, 720, nullptr, &frame);
			kinc_set_update_callback(update);
			kinc_set_shutdown_callback(shutdown);

			#ifdef KISS_IMGUI
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

				// Setup Dear ImGui style
				ImGui::StyleColorsDark();
				//ImGui::StyleColorsLight();

				// Setup Platform/Renderer bindings
				ImGui_ImplKinc_InitForG4(0);
				ImGui_ImplG4_Init(0);
			#endif
		}

		void setResolution(const u32 width, const u32 height, const u32 samples, const u32 zBits, const u32 stencilBits)
		{
			kinc_window_resize(0, width, height);
			gfx2d::init();
			gfx2d::on_resize((float)width, (float)height);
			kinc_framebuffer_options frame;
			frame.vertical_sync = false;//true or false commenting this entirely kills release performances!!!
			frame.depth_bits = zBits;
			frame.samples_per_pixel = samples;
			frame.stencil_bits = stencilBits;
			kinc_window_change_framebuffer(0, &frame);
			kinc_window_set_resize_callback(0, resize, 0);
			kinc_g4_restore_render_target();
		}
	}
}

int kickstart(int argc, char** argv) 
{
	auto result = kiss::app::on_launch(argc, argv);
	if (result == 0) 
	{
		kinc_start();
	}
	return result;
}