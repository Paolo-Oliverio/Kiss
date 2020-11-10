#pragma once
#include <Kiss/pch.h>
#include <Kore/Math/Matrix.h>
//#include "gfx2dDefs.h"

namespace kiss 
{
	namespace gfx2d 
	{
		void init();
		void resize(float w, float h, float Scale);
		void shutdown();

		namespace matrices 
		{
			inline Kore::mat3 projection(float w, float h) 
			{
				return Kore::mat3::Scale(1.f / w / 0.5f, -1.f / h / 0.5f, 1.f) * Kore::mat3::Translation(-w * 0.5f, -h * 0.5f);
			}
		}
		
	}
}