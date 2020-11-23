// dear imgui: Renderer for G4

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#pragma once

namespace kiss {
	namespace imgui {
		namespace gfx {
			void init();
			void release();
			void prerender();
			void render(ImDrawData* draw_data);
		}
	}
}