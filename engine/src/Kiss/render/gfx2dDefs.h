#pragma once
#include <Kiss/pch.h>
#include <Kiss/math/v2.h>

namespace kiss {
	struct atlas;
	template <typename Vtx, typename VData>
	class quadBatcher;

	struct textCtx : v2 {
		float start_x;
		float w;
		textCtx(float x, float y, float w = 1000) : v2(x, y), start_x(x), w(w) {}
		textCtx() : v2(0, 0), start_x(0), w(1000) {}
	};

	namespace gfx2d {

		namespace vertex {
			struct pos_uv_color {
				v2	pos;
				s16	u;
				s16	v;
				u32	color;
				pos_uv_color(float x, float y, s16 u, s16 v, u32 color) : pos(x, y), u(u), v(v), color(color) {};
			};
			namespace structure {
				extern gx::VertexStructure pos_uv_color;
			}
		}

		namespace quad {
			typedef quadBatcher<vertex::pos_uv_color, u32> colored;
			extern  colored* batcher;
			extern  gx::IndexBuffer* ibuffer;
			extern  gx::PipelineState* pipe;
			namespace atlases {
				extern atlas* gui;
				extern atlas* sprites;
			}
		}

		namespace matrices {
			extern m3 proj;// updated from the camera on set
		}

		namespace screen {
			extern gx::PipelineState* pipe;
			extern gx::VertexBuffer* vbuffer;
		}
	}
}