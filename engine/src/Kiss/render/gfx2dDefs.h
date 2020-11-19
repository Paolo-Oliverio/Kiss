#pragma once
#include <Kiss/pch.h>
#include <Kiss/math/v2.h>

FWD_G4(pipeline);
FWD_G4(texture_unit);
FWD_G4(constant_location);
FWD_G4(vertex_structure);
FWD_G4(index_buffer);
FWD_G4(shader);

FWD_KINC(matrix3x3);

namespace kiss 
{
	struct atlas;
	template <typename Vtx, typename VData, VData vDataDef>
	class quadBatcher;
	struct vbRing;
	struct ibRing;

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
		
		typedef quadBatcher<vertex::pos_uv_color, u32, 0xFFFFFFFF> defaultBatcher;

		class pipeline2d
		{	
			public:
				kinc_g4_pipeline_t				pipe;
				kinc_g4_texture_unit_t			texture_unit;
				kinc_g4_constant_location_t		proj_location;
				vbRing*							bufferManager;
				u8								vertexSize;
			void init(kinc_g4_shader_t* vshader, kinc_g4_shader_t* fshader, kinc_g4_vertex_structure_t* vlayout);
			void release();
		};
		
		extern float							scaling;
		extern defaultBatcher					batcher;
		extern atlas*							atlas0;

		namespace base {
			extern pipeline2d					pipeline;
			extern kinc_g4_vertex_structure_t	vlayout;
			extern kinc_g4_shader_t				vshader;
			extern kinc_g4_shader_t				fshader;
		}

		namespace vbuffer {
			extern vbRing						manager;
		}

		namespace ibuffer {
			extern  kinc_g4_index_buffer_t		quads;
			extern  ibRing						manager;
		}

		namespace xform {
			extern kinc_matrix3x3_t				gui;
			extern kinc_matrix3x3_t				sprite;
		}
		
		void init();
	}
}