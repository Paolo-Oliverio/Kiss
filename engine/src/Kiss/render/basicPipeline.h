#pragma once
#include <Kiss/pch.h>
#include <kinc/math/matrix.h>
//#include "gfx2dDefs.h"

namespace kiss 
{
	namespace gfx2d 
	{
		void init();
		void on_resize(float w, float h);
		void shutdown();

		namespace matrices 
		{
			inline void projection(kinc_matrix3x3_t& proj, const float w, const float h)
			{
				const float tw =  (w * 0.5f);
				const float th =  (h * 0.5f);
				const float sw =  1 / tw;
				const float sh = -1 / th;
				proj = {
					sw			, 0.f			, 0,
					0.f			, sh			, 0,
					-tw * sw	, -th * sh		, 1,
				};
			}
		}
	}
}