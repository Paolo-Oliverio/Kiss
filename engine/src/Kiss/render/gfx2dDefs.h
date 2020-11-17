#pragma once
#include <Kiss/pch.h>
#include <Kiss/math/v2.h>

FwdStruct(kinc_g4_pipeline);
FwdStruct(kinc_g4_texture_unit);
FwdStruct(kinc_g4_constant_location);
FwdStruct(kinc_g4_vertex_structure);
FwdStruct(kinc_g4_index_buffer);

namespace kiss 
{
	struct atlas;
	template <typename Vtx, typename VData, VData vDataDef>
	class quadBatcher;

	struct textCtx : v2 
	{
		float start_x;
		float w;
		textCtx(float x, float y, float w = 1000) : v2(x, y), start_x(x), w(w) {}
		textCtx() : v2(0, 0), start_x(0), w(1000) {}
	};

	namespace gfx2d 
	{
		namespace vertex 
		{
			struct pos_uv_color 
			{
				v2	pos;
				s16	u;
				s16	v;
				u32	color;
				pos_uv_color(float x, float y, s16 u, s16 v, u32 color) : pos(x, y), u(u), v(v), color(color) {};
			};
		}

		class basicPipe
		{	
			public:
				kinc_g4_pipeline_t			pipe;
				kinc_g4_texture_unit_t		texture_unit;
				kinc_g4_constant_location_t	proj_location;
				kinc_g4_vertex_structure_t	vertexLayout;
				kinc_matrix3x3_t			guiProjection;
				kinc_matrix3x3_t			spriteProjection;
				float						scaling = 1.f;
			void init();
		};

		extern basicPipe Pipe2d;

		namespace quad 
		{
			typedef quadBatcher<vertex::pos_uv_color, u32, 0xFFFFFFFF> colored;
			extern  colored* batcher;
			extern  kinc_g4_index_buffer_t* ibuffer;
			namespace atlases {
				extern atlas* gui;
				extern atlas* sprites;
			}
		}
	}
}