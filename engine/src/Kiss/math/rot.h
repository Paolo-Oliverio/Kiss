#pragma once 
#include "math.h"
#include "v2.h"


namespace kiss {
	// 2d rot composed of cos/sin pair
	struct rot {
		f32 c;
		f32 s;
		rot() :c(1.f), s(0.f) {}
		rot(f32 radians) : c(cosf(radians)), s(sinf(radians)) {}
		rot(f32 c, f32 s) :c(c), s(s) {}

		
		inline f32 get_angle()const { return atan2f(s, c); }
		inline v2 get_x_axis() { return v2(c, s); }
		inline v2 get_y_axis() { return v2(s, c); }
		v2 to_v2() { return v2(c, s); }
	};
	// rot ops

	inline v2 operator*(rot a, v2 b) { return v2(a.c * b.x - a.s * b.y, a.s * b.x + a.c * b.y); }
	inline rot operator*(rot a, rot b) { rot c; c.c = a.c * b.c - a.s * b.s; c.s = a.s * b.c + a.c * b.s; return c; }
	
	//Lookup table;
	//extern rot rotation_lookup[rotation_lookup_size];

	inline constexpr f32 deg2rad(f32 n) {
		return (n * cPi) / 180;
	}

	inline constexpr f32 rad2deg(f32 n) {
		return (n * 180) / cPi;
	}
}