#pragma once

#include "v2.h"

namespace kiss {
	struct aabb {
		v2 min;
		v2 max;

		inline aabb( v2* verts, const int count) {
			v2 lmin = verts[0];
			v2 lmax = lmin;
			for (int i = 0; i < count; ++i) {
				lmin = v2::min(lmin, verts[i]);
				lmax = v2::max(lmax, verts[i]);
			}
			min = lmin;
			max = lmax;
		}

		inline aabb( v2 min,  v2 max) :min(min), max(max) {}

		inline aabb( f32 xmin,  f32 ymin,  f32 xmax,  f32 ymax) : min(xmin, ymin), max(xmax, ymax) {}

		static aabb fromCenter( v2 center,  v2 halfExtents)	
		{ 
			return aabb(center - halfExtents, center + halfExtents); 
		}

		static inline aabb clamp(aabb a, aabb b)
		{ 
			return aabb(kiss::clamp(a.min, b.min, b.max),kiss::clamp(a.max, b.min, b.max));
		}
		
		inline aabb operator*(f32 b) { return aabb(min * b, min * b); }

		//inline aabb( circle c) { *this = fromCenter(c.p, v2(c.r, c.r));}
		inline void addX( f32 x) { min.x += x; max.x += x; }
		inline void addY( f32 y) { min.y += y; max.y += y; }

		inline aabb offset( v2 offset)  { return aabb(min + offset, max + offset); }
		inline aabb merge(v2 p) { return aabb(v2::min(min, p), v2::max(max, p)); }
		inline aabb merge( aabb b)  { return aabb(v2::min(min, b.min), v2::max(max, b.max)); }

		inline void expandX	(f32 x)	{ min.x -= x;  max.x += x; }
		inline void expandY	(f32 y)	{ min.y -= y;  max.y += y; }
		inline void expand	(f32 xy){ min -= xy; max += xy; }		
		
		inline v2 halfExtents()	const	{ return (max - min) * 0.5f; };
		inline v2 clamp( v2 p)	const	{ return kiss::clamp(p, min, max); }
		inline v2 center()		const	{ return (min + max) * 0.5f; }
		inline v2 top_left()	const	{ return min; }
		inline v2 top_right()	const	{ return v2(max.x, min.y); }
		inline v2 bottom_left()	const	{ return v2(min.x, max.y); }
		inline v2 bottom_right()const	{ return max; }

		inline f32 mergedArea( aabb b)const { return (kinc_max(max.x, b.max.x) - kinc_min(min.x, b.min.x))*(kinc_max(max.y, b.max.y) - kinc_min(min.y,b.min.y)); }
		inline f32 width()		const { return max.x - min.x; }
		inline f32 height()		const { return max.y - min.y; }
		inline f32 halfWidth()	const { return width() * 0.5f;}
		inline f32 halfHeight()	const { return height() * 0.5f; }
		inline f32 area()const { return width() * height(); }

		inline bool intersects( aabb b) const {return (min.x <= b.max.x && b.min.x <= max.x && min.y <= b.max.y && b.min.y <= max.y);}
		inline bool contains(aabb b)	const {return min >= b.min && max <= b.max; }
		inline bool contains( v2 p)		const {return p >= min && p <= max; }

		inline void getVertices(v2* out)  {
			out[0] = bottom_left();
			out[1] = bottom_right();
			out[2] = top_left();
			out[3] = top_right();
		}
		/// Wrap a vector to a bounding box.
		inline v2 wrap( v2 v) 
		{
			const f32 dx	= kinc_abs(width());
			const f32 modx	= kinc_mod(v.x - min.x, dx);
			const f32 x		= (modx > 0.0f) ? modx : modx + dx;
			const f32 dy	= kinc_abs(height());
			const f32 mody	= kinc_mod(v.y - min.y, dy);
			const f32 y		= (mody > 0.0f) ? mody : mody + dy;
			return v2(x + min.x, y + min.y);
		}

		/*
		/// Returns the fraction along the segment query the cpBB is hit. Returns INFINITY if it doesn't hit.
		inline bool SegmentHit( v2 a,  v2 b , fp & t) {
			v2 delta = b - a;
			fp tmin = -INFINITY, tmax = INFINITY;
			if (delta.x == 0.0f) {
				if (a.x < min.x || max.x < a.x){ t = INFINITY; return false; } 
			}
			else {
				fp t1 = (min.x - a.x) / delta.x;
				fp t2 = (max.x - a.x) / delta.x;
				tmin = ::max(tmin, ::min(t1, t2));
				tmax = ::min(tmax, ::max(t1, t2));
			}

			if (delta.y == 0.0f) {
				if (a.y < min.y || max.y < a.y) { t = INFINITY; return false; };
			}
			else {
				fp t1 = (min.y - a.y) / delta.y;
				fp t2 = (max.y - a.y) / delta.y;
				tmin = ::max(tmin, ::min(t1, t2));
				tmax = ::min(tmax, ::max(t1, t2));
			}

			if (tmin <= tmax && 0.0f <= tmax && tmin <= 1.0f) {
				t = ::max(tmin, 0.0f);
				return true;
			} else { t = INFINITY; return false; };
		}*/
	};
}