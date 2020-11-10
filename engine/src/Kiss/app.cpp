#include "app.h"
#include "time/delta.h"
#include <Kiss/render/basicPipeline.h>
#include <kinc/graphics4/graphics.h>
#include <Kore/System.h>


//#include "sound/soundSystem.h"
#ifdef KISS_IMGUI
	#include <imgui.h>
	#include <imgui_impl_kinc.h>
	#include <imgui_impl_g4.h>
#endif

using namespace Kore;

namespace kiss {

	namespace framework {

		static inline void render(float dt) 
		{
			kinc_g4_begin(0);
			app::render(dt);
			#ifdef KISS_IMGUI
				// Start the Dear ImGui frame
				ImGui_ImplG4_NewFrame();
				ImGui_ImplKinc_NewFrame(0);
				ImGui::NewFrame();
				app::imgui(dt);
				ImGui::Render();
				ImGui_ImplG4_RenderDrawData(ImGui::GetDrawData());
			#endif
			kinc_g4_end(0);
			kinc_g4_swap_buffers();
		}

		void update() 
		{
			auto dt = time.update();
			app::update(dt);
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

			app::shutdown();
			gfx2d::shutdown();
		}

		void init(const char* title) 
		{
			FramebufferOptions frame;
			frame.verticalSync = false;
			frame.depthBufferBits = 0;
			frame.samplesPerPixel = 1;
			frame.stencilBufferBits = 0;
			System::init(title, 1280, 720,nullptr, &frame);
			System::setCallback(update);
			System::setShutdownCallback(shutdown);

			#ifdef KISS_IMGUI
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

				// Setup Dear ImGui style
				ImGui::StyleColorsDark();

				// Setup Platform/Renderer bindings
				ImGui_ImplKinc_InitForG4(0);
				ImGui_ImplG4_Init(0);
			#endif
		}

		void setResolution(const u32 width, const u32 height, const f32 scale, const u32 samples, const u32 zBits, const u32 stencilBits)
		{
			auto window = Window::get(0);
			window->resize(width, height);
			gfx2d::init();
			gfx2d::resize(width,height,scale);
			FramebufferOptions frame;
			frame.depthBufferBits = zBits;
			frame.samplesPerPixel = samples;
			frame.stencilBufferBits = stencilBits;
			frame.verticalSync = false;
			window->changeFramebuffer(&frame);
			window->setResizeCallback(app::resize);
		}
	}
}

int kickstart(int argc, char** argv) 
{
	auto result = kiss::app::main(argc, argv);
	if (result == 0) 
	{
		System::start();
	}
	return result;
}