#include <Kiss/data/atlas.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/shader.h>
#include <kinc/graphics4/textureunit.h>
#include <kinc/io/filereader.h>
#include "basicPipeline.h"
#include "quadbatcher.h"
#include <malloc.h>

namespace kiss 
{
	constexpr int	max_quads_v = 0xFFFF; // Max 65535 vertices per buffer.
	constexpr int	max_quads = max_quads_v / 4;
	constexpr int	max_quads_i = max_quads * 6;
	constexpr int	buffers_num = 4;

	void load_shader(const char* filename, kinc_g4_shader_t* shader, kinc_g4_shader_type_t shader_type)
	{
		kinc_file_reader_t file;
		kinc_file_reader_open(&file, filename, KINC_FILE_TYPE_ASSET);
		size_t data_size = kinc_file_reader_size(&file);
		uint8_t* data = (uint8_t*)alloca(data_size);
		kinc_file_reader_read(&file, data, data_size);
		kinc_file_reader_close(&file);
		kinc_g4_shader_init(shader, data, data_size, shader_type);
	}

	namespace gfx2d 
	{	
		float						scaling = 1.0f;
		defaultBatcher				batcher;
		atlas*						atlas0 = nullptr;
		
		namespace base {
			pipeline2d					pipeline;
			kinc_g4_vertex_structure_t	vlayout;
			kinc_g4_shader_t			vshader;
			kinc_g4_shader_t			fshader;

			static inline void init(const char* vshader_name, const char* fshader_name) {
				// Create the input layout //--------------------------------------------------------
				kinc_g4_vertex_structure_init(&vlayout);
				kinc_g4_vertex_structure_add(&vlayout, "pos", KINC_G4_VERTEX_DATA_FLOAT2);
				kinc_g4_vertex_structure_add(&vlayout, "tex", KINC_G4_VERTEX_DATA_SHORT2_NORM);
				kinc_g4_vertex_structure_add(&vlayout, "col", KINC_G4_VERTEX_DATA_COLOR);
				//-----------------------------------------------------------------------------------
				load_shader(vshader_name, &vshader, KINC_G4_SHADER_TYPE_VERTEX);
				load_shader(fshader_name, &fshader, KINC_G4_SHADER_TYPE_FRAGMENT);
				//------------------------------------------------------------------------------------
				pipeline.init(&vshader, &fshader, &vlayout);
			}

			static inline void release() {
				pipeline.release();
				kinc_g4_shader_destroy(&vshader);
				kinc_g4_shader_destroy(&fshader);
			}
		}

		namespace vbuffer {

			vbRing	manager;

			static inline void init(u32 size, u8 num) {
				manager.init(&base::vlayout, size, num);
			}

			static inline void release() {
				manager.release();
			}
		}
		
		namespace ibuffer
		{
			ibRing					manager;
			kinc_g4_index_buffer_t	quads;

			static void init(int size, int num) 
			{
				manager.init(size, num);
				//-------------------------------------------------------------------------------------------
				kinc_g4_index_buffer_init(&quads, max_quads_i, KINC_G4_INDEX_BUFFER_FORMAT_16BIT);
				auto ndx = kinc_g4_index_buffer_lock(&quads);
				for (int i = 0; i < max_quads; ++i)
				{
					ndx[0] = i * 4;
					ndx[1] = i * 4 + 1;
					ndx[2] = i * 4 + 2;
					ndx[3] = i * 4 + 1;
					ndx[4] = i * 4 + 3;
					ndx[5] = i * 4 + 2;
					ndx += 6;
				}
				kinc_g4_index_buffer_unlock(&quads);
			}

			static void release() 
			{
				kinc_g4_index_buffer_destroy(&quads);
				manager.release();
			}
		}
		
		kinc_matrix3x3_t			xform::gui;
		kinc_matrix3x3_t			xform::sprite;


		namespace atlases
		{
			void init()
			{
				atlas0 = new atlas("Gui", ".png");
			}

			void release()
			{
				delete atlas0;
			}
		}

		void pipeline2d::init(kinc_g4_shader_t* vshader, kinc_g4_shader_t* fshader, kinc_g4_vertex_structure_t* vlayout)
		{
			kinc_g4_pipeline_init(&pipe);
			//-----------------------------------------------------------------------------------
			pipe.vertex_shader = vshader;
			pipe.fragment_shader = fshader;
			//-----------------------------------------------------------------------------------
			pipe.input_layout[0] = &base::vlayout;
			pipe.input_layout[1] = nullptr;
			//-----------------------------------------------------------------------------------
			pipe.blend_source				= KINC_G4_BLEND_SOURCE_ALPHA;
			pipe.blend_destination			= KINC_G4_BLEND_INV_SOURCE_ALPHA;
			pipe.alpha_blend_source			= KINC_G4_BLEND_SOURCE_ALPHA;
			pipe.alpha_blend_destination	= KINC_G4_BLEND_INV_SOURCE_ALPHA;
			//-----------------------------------------------------------------------------------
			kinc_g4_pipeline_compile(&pipe);
			//-----------------------------------------------------------------------------------
			texture_unit	= kinc_g4_pipeline_get_texture_unit(&pipe,"texsampler");
			proj_location	= kinc_g4_pipeline_get_constant_location(&pipe,"xform");
			bufferManager	= &vbuffer::manager;
		}

		void pipeline2d::release() {
			kinc_g4_pipeline_destroy(&pipe);
		}


		void init() 
		{
			base::init("baseSprite.vert", "baseSprite.frag");
			ibuffer::init(max_quads_i, buffers_num);
			vbuffer::init(max_quads_v, buffers_num);
			atlases::init();
		}

		void on_resize(float w, float h)
		{
			matrices::projection(xform::sprite, w / scaling, h / scaling);
			matrices::projection(xform::gui, w, h);
		}

		void release() 
		{
			atlases::release();
			vbuffer::release();
			ibuffer::release();
			base::release();
		}
	}
}
