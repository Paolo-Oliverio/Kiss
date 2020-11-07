#pragma once
#include "math.h"

namespace kiss {
	struct v2 {
		f32 x;
		f32 y;
		v2() {}
		v2(f32 x, f32 y) : x(x), y(y) {}

		static v2 zero() { return v2(0.f, 0.f); }
		static inline v2 fromAngle( f32 a)	{ return v2(math::cos(a), math::sin(a)); }

		static inline v2 min(v2 a, v2 b) { return v2(math::min(a.x, b.x), math::min(a.y, b.y)); }
		static inline v2 max(v2 a, v2 b) { return v2(math::max(a.x, b.x), math::max(a.y, b.y)); }

		inline void set(f32 b) { x = y = b; }
		inline void set(f32 nx, f32 ny) { x = nx; y = ny; }

		inline void operator +=	(v2 b)	{ x += b.x; y += b.y; }
		inline void operator -=	(v2 b)	{ x -= b.x; y -= b.y; }
		inline void operator *=	(v2 b)	{ x *= b.x; y *= b.y; }
		inline void operator +=	(f32 b) { x += b; y += b; }
		inline void operator -=	(f32 b) { x -= b; y -= b; }
		inline void operator *=	(f32 b) { x *= b; y *= y; }
		inline void operator /=	(f32 b) { x /= b; y /= b; }

		inline f32 min() { return math::min(x, y); }
		inline f32 max() { return math::max(x, y); }

		inline v2 cw90()	{ return v2(-y, x); }
		inline v2 ccw90()	{ return v2(y, -x); }
		inline v2 abs()		{ return v2(math::abs(x), math::abs(y)); }
		inline f32 get_angle()				{ return math::atan2(y, x); }
		
		inline f32 len() { return math::sqrt(x * x + y * y); }
		inline f32 lenSquared() { return (x * x + y * y); }
		inline f32 invLen() { return 1 / (len() + cEpsilon); }
		inline v2 normalized() {
			float invLen = 1.f / (len() + cEpsilon);
			return v2(x * invLen, y * invLen);
		}
	};

	// vector ops
	
	//inline v2 operator -(v2 b)  { return v2(this->x - b.x, this->y - b.y); }
	inline v2 operator+( v2 a,  v2 b)		{ return v2(a.x + b.x, a.y + b.y); }
	inline v2 operator-( v2 a,  v2 b)		{ return v2(a.x - b.x, a.y + b.y); }
	inline v2 operator*( v2 a,  f32 b)	{ return v2(a.x * b, a.y * b); }
	inline v2 operator*( v2 a,  v2 b)		{ return v2(a.x * b.x, a.y * b.y); }
	inline v2 operator/( v2 a,  f32 b)	{ return v2(a.x / b, a.y / b); }

	inline f32	det2( v2 a,  v2 b)				{ return a.x * b.y - a.y * b.x; }
	inline f32	dot( v2 a,  v2 b)					{ return a.x * b.x + a.y * b.y; }
	inline v2	clamp( v2 a,  v2 lo,  v2 hi)	{ return v2::max(lo, v2::min(a, hi)); }
	inline f32	cross( v2 v1,  v2 v2)				{ return v1.x * v2.y - v1.y * v2.x; }
	
	inline v2	lerp( v2 a,  v2 b,  f32 t)	{ return a + (b - a) * t; }
	inline v2	operator-(v2 a) {return v2(-a.x, -a.y); }

	inline int operator<( v2 a,  v2 b)	{ return a.x < b.x && a.y < b.y; }
	inline int operator>( v2 a,  v2 b)	{ return a.x > b.x && a.y > b.y; }
	inline int operator<=( v2 a,  v2 b)	{ return a.x <= b.x && a.y <= b.y; }
	inline int operator>=( v2 a,  v2 b)	{ return a.x >= b.x && a.y >= b.y; }
	inline v2 project( v2 v1,  v2 v2){return v2 * (dot(v1, v2) / dot(v2, v2));}
	/// Uses complex number multiplication to rotate v1 by v2. Scaling will occur if v1 is not a unit vector.
	static inline v2 rotate( v2 a,  v2 b){return v2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);}
	/// Inverse of cpvrotate().
	static inline v2 unrotate( v2 a,  v2 b){return v2(a.x * b.x + a.y * b.y, a.y * b.x - a.x * b.y);}

	inline v2 closestPointOnSegment(v2 p,v2 a, v2 b) {
		v2 delta = a - b;
		f32 t = math::clamp(dot(delta, (p - b)) / delta.lenSquared(), 0.f, 1.f);
		return b + (delta * t);
	}
	inline int parallel(v2 a, v2 b, f32 tol)
	{
		f32 k = a.len() / b.len();
		b = b * k;
		if (math::abs(a.x - b.x) < tol && math::abs(a.y - b.y) < tol) return 1;
		return 0;
	}
}