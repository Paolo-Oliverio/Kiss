#pragma once
#include <Kiss/pch.h>

namespace kiss {
	namespace app {
		//They need to be implemented in the project;
		int	 start(int argc, char** argv);
		void input(float dt);
		void prePhysics(float dt);
		void postPhysics(float dt);
		void render(float dt);
		#ifdef KISS_IMGUI
			void gui(float dt);
		#endif
		void resize(int x, int y);
		void release();
	}

	namespace framework {
		//Only call from Project::Main
		void init(const char* title);
		void setResolution(const u32 width, const u32 height, const u32 samples = 1, const u32 zBits = 0, const u32 stencilBits = 0);
	}
}