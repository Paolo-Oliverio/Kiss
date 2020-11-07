#pragma once 
#include "math.h"

namespace kiss {
	// 2d rot composed of cos/sin pair
	struct rot {
		f32 c;
		f32 s;
		rot() :s(0.f), c(1.f) {}
		rot(f32 radians) :s(math::sin(radians)), c(math::cos(radians)) {}
		inline f32 get_angle()const { return math::atan2(s, c); }
		inline v2 get_x_axis() { return v2(c, s); }
		inline v2 get_y_axis() { return v2(s, c); }
		v2 to_v2() { return v2(c, s); }
	};
	// rot ops

	inline v2 operator*(rot a, v2 b) { return v2(a.c * b.x - a.s * b.y, a.s * b.x + a.c * b.y); }
	inline rot operator*(rot a, rot b) { rot c; c.c = a.c * b.c - a.s * b.s; c.s = a.s * b.c + a.c * b.s; return c; }
	
	//Lookup table;
	//extern rot rotation_lookup[rotation_lookup_size];

	inline f32 deg2rad(f32 n) {
		return (n * cPi) / 180;
	}

	inline f32 rad2deg(f32 n) {
		return (n * 180) / cPi;
	}
}