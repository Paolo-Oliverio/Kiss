#pragma once
#include <Kiss/pch.h>
#include "gfx2dDefs.h"

namespace kiss {
	namespace gfx2d {
		void init(float width, float height);
		void shutdown();

		namespace matrices {
			inline void projection(float w, float h) {
				proj = Kore::mat3::Scale(1.f / w / 0.5f, -1.f / h / 0.5f, 1.f) * Kore::mat3::Translation(-w * 0.5f, -h * 0.5f);
			}
		}
	}
}