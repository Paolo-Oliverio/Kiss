#pragma once
#include "v2.h"

namespace kiss {

	inline v2 intersect(v2 a, v2 b, f32 da, f32 db) { return a + (b - a) * (da / (da - db)); }

	struct raycast {
		f32 t;	// toi
		v2 n;   // normalized normal at impact point
	};

	struct ray {
		v2 p;   // position
		v2 d;   // direction (normalized)
		f32 l;	// ray lenght
		inline v2 impact(f32 tt)const { return p + d * tt; }
	};
}