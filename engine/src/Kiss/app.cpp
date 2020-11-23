#include "app.h"
#include "time/delta.h"
#include <Kiss/render/basicPipeline.h>
#include <kinc/graphics4/graphics.h>
#include <kinc/system.h>
#include <kinc/window.h>

//#include "sound/soundSystem.h"
#ifdef KISS_IMGUI
	#include <imgui_kiss.h>
#endif

#ifdef KISS_SOLOUD
	#include <Kiss/sound/sound.h>
#endif

namespace kiss 
{
	namespace framework 
	{
		static inline void render(float dt) 
		{
			kinc_g4_begin(0);
			app::render(dt);
			WithImgui(imgui::render(dt));
			kinc_g4_end(0);
			kinc_g4_swap_buffers();
		}

		void update() 
		{
			auto dt = time.update();
			app::prePhysics(dt);
			app::postPhysics(dt);
			render(dt);
		}

		void release() 
		{
			WithImgui(imgui::release());
			WithSoloud(sound::init());
			app::release();
			gfx2d::release();
		}

		void resize(int x, int y, void* data)
		{
			#ifdef KORE_OPENGL
				kinc_g4_restore_render_target();
			#endif
			gfx2d::on_resize((float)x, (float)y);
			app::resize(x,y);
			WithImgui(imgui::resize(x, y));
		}

		void init(const char* title) 
		{
			{//window
				kinc_framebuffer_options frame;
				frame.vertical_sync = false;
				frame.depth_bits = 0;
				frame.samples_per_pixel = 1;
				frame.stencil_bits = 0;
				kinc_init(title, 1280, 720, nullptr, &frame);
			}

			kinc_set_update_callback(update);
			kinc_set_shutdown_callback(release);

			//libraries init
			WithSoloud(sound::init());
			WithImgui(imgui::init());
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
			WithImgui(imgui::resize(width, height));
		}
	}
}

int kickstart(int argc, char** argv) 
{
	auto result = kiss::app::start(argc, argv);
	if (result == 0) 
	{
		kinc_start();
	}
	return result;
}