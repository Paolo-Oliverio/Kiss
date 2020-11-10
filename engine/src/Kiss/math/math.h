#pragma once
#include <Kiss/pch.h>

namespace kiss {
	constexpr f32 cPi = 3.14159265358979323846f;
	constexpr f32 cEpsilon = 0.000000000001f;

	inline f32 lerp_fast(f32 v0, f32 v1, f32 t) {
		return v0 + t * (v1 - v0);
	}

	// Precise method, v = v1 when t = 1.
	inline f32 lerp_precise(f32 v0, f32 v1, f32 t) {
		return (1 - t) * v0 + t * v1;
	}
}