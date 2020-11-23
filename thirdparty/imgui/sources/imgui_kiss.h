#pragma once

namespace kiss
{
	namespace imgui 
	{
		void init();
		void render(float dt);
		void resize(int w, int h);
		void release();
	}
}