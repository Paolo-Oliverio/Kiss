#pragma once
#include <Kiss/pch.h>

namespace kiss {
	namespace app {
		//They need to be implemented in the project;
		int	 on_launch(int argc, char** argv);
		void on_update(float dt);
		void on_render(float dt);
		#ifdef KISS_IMGUI
			void on_imgui(float dt);
		#endif
		void on_resize(int x, int y);
		void on_shutdown();
	}

	namespace framework {
		//Only call from Project::Main
		//void init(const char* title, const u32 width, const u32 height, const bool fullscreen = 0, const f32 scale = 1.f, const u32 zBits = 0, const u32 stencilBits = 0);
		void init(const char* title);
		void setResolution(const u32 width, const u32 height, const u32 samples = 1, const u32 zBits = 0, const u32 stencilBits = 0);
	}
}