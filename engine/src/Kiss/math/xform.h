#pragma once
#include "v2.h"
#include "aabb.h"

namespace kiss {
	/// Column major affine transform.
	typedef struct xform2d {
		f32 a, b, c, d, tx, ty;
		//xform2d(const fp a, const  fp b, const  fp c, const fp d, const fp tx, const fp ty) :a(a), b(b), c(c), d(d), tx(tx), ty(ty) {}
		/// Construct a new transform matrix in transposed order.
		xform2d (const f32 a, const f32 c, const f32 tx, const f32 b, const f32 d, const f32 ty):a(a), b(b), c(c), d(d), tx(tx), ty(ty) {}
		/// Get the inverse of a transform matrix.
		static inline xform2d inverse(const xform2d t) {
			const f32 inv_det = 1.0 / (t.a*t.d - t.c*t.b);
			return xform2d(
				t.d*inv_det, -t.c*inv_det, (t.c*t.ty - t.tx*t.d)*inv_det,
				-t.b*inv_det, t.a*inv_det, (t.tx*t.b - t.a*t.ty)*inv_det
			);
		}
		/// Transform an absolute point. (i.e. a vertex)
		inline v2 operator*(v2 p) {
			return v2(a * p.x + c * p.y + tx, b * p.x + d * p.y + ty);
		}
		/// Transform a vector (i.e. a normal)
		inline v2 xformVect(v2 v) {
			return v2(a * v.x + c * v.y, b * v.x + d * v.y);
		}
		/// Create a transation matrix.
		static inline xform2d translate(v2 translate) {
			return xform2d(
				1.0, 0.0, translate.x,
				0.0, 1.0, translate.y
			);
		}
		/// Create a scale matrix.
		static inline xform2d scale(f32 scaleX, f32 scaleY) {
			return xform2d(
				scaleX, 0.0, 0.0,
				0.0, scaleY, 0.0
			);
		}
		/// Create a rot matrix.
		static inline xform2d rotate(f32 radians) {
			v2 rot = v2::fromAngle(radians);
			return xform2d(
				rot.x, -rot.y, 0.0,
				rot.y, rot.x, 0.0
			);
		}
		/// Create a rigid transformation matrix. (transation + rot)
		static inline xform2d rigid(v2 translate, f32 radians) {
			v2 rot = v2::fromAngle(radians);
			return xform2d(
				rot.x, -rot.y, translate.x,
				rot.y, rot.x, translate.y
			);
		}
		/// Fast inverse of a rigid transformation matrix.
		static inline xform2d rigidInverse(xform2d t) {
			return xform2d(
				t.d, -t.c, (t.c*t.ty - t.tx*t.d),
				-t.b, t.a, (t.tx*t.b - t.a*t.ty)
			);
		}

		static inline xform2d WrapInverse(xform2d outer, xform2d inner)
		{
			return outer * (inner * inverse(outer));
		}

		static inline xform2d cpTransformWrap(xform2d outer, xform2d inner)
		{
			return inverse(outer) * (inner * outer);
		}

		static inline xform2d Ortho(const aabb bb)
		{
			const f32 w = bb.width();
			const f32 h = bb.height();
			return xform2d(
				(2.0 / w), (0.0), (-(bb.max.x + bb.min.x) / w),
				0.0, (2.0 / h), (-(bb.max.y + bb.min.y) / h)
			);
		}

		static inline xform2d BoneScale(v2 a, v2 b) {
			const v2 d = b - a;
			return xform2d(
				d.x, -d.y, a.x,
				d.y, d.x, a.y
			);
		}

		static inline xform2d AxialScale(v2 axis, v2 pivot, f32 scale) {
			const f32 A = axis.x * axis.y *(scale - 1.f);
			const f32 B = dot(axis, pivot)*(1.f - scale);
			return xform2d(
				scale * axis.x * axis.x + axis.y * axis.y, A, axis.x * B,
				A, axis.x * axis.x + scale * axis.y * axis.y, axis.y * B
			);
		}
	};

	/// Multiply two transformation matrices.
	inline xform2d operator*(const xform2d t1, const xform2d t2) {
		return xform2d(
			t1.a * t2.a + t1.c * t2.b, t1.a * t2.c + t1.c * t2.d, t1.a * t2.tx + t1.c * t2.ty + t1.tx,
			t1.b * t2.a + t1.d * t2.b, t1.b * t2.c + t1.d * t2.d, t1.b * t2.tx + t1.d * t2.ty + t1.ty
		);
	}

	/// Transform a aabb.
	inline aabb operator*(xform2d t, aabb bb) {
		v2 center = bb.center();
		const f32 hw = bb.halfWidth();
		const f32 hh = bb.halfHeight();
		f32 a = t.a * hw, b = t.c * hh, d = t.b * hw, e = t.d * hh;
		f32 hw_max = math::max(math::abs(a + b), math::abs(a - b));
		f32 hh_max = math::max(math::abs(d + e), math::abs(d - e));
		return aabb::fromCenter(t * center, v2(hw_max, hh_max));
	}
}