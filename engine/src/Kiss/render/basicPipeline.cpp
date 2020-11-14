#include <Kiss/data/atlas.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/shader.h>
#include <kinc/graphics4/textureunit.h>
#include <kinc/io/filereader.h>
#include "basicPipeline.h"
#include "quadbatcher.h"

namespace {
	kinc_g4_shader_t basicVertexShader;
	kinc_g4_shader_t basicFragmentShader;
}
using namespace Kore;

namespace kiss 
{
	namespace gfx2d 
	{
		namespace vertex 
		{
			namespace structure 
			{
				kinc_g4_vertex_structure pos_uv_color;
			}
		}

		namespace quad 
		{
			kinc_g4_index_buffer_t* ibuffer = nullptr;
			colored* batcher = nullptr;

			void init_ibuffer();
			void init();
			void free();

			namespace atlases 
			{
				atlas* gui = nullptr;
				atlas* sprites = nullptr;
				void free(atlas* atlas);
				void load(atlas*& a, const char* name);
				void free_all();
				void load_all();
			}
		}

		static void load_shader(const char* filename, kinc_g4_shader_t* shader, kinc_g4_shader_type_t shader_type)
		{
			kinc_file_reader_t file;
			kinc_file_reader_open(&file, filename, KINC_FILE_TYPE_ASSET);
			size_t data_size = kinc_file_reader_size(&file);
			uint8_t* data = (uint8_t*)malloc(data_size);
			kinc_file_reader_read(&file, data, data_size);
			kinc_file_reader_close(&file);
			kinc_g4_shader_init(shader, data, data_size, shader_type);
			free(data);
		}

		void basicPipe::init() 
		{
			kinc_g4_pipeline_init(&pipe);
			//auto shaderfile = FileReader();
			//-----------------------------------------------------------------------------------
			load_shader("baseSprite.vert", &basicVertexShader,	KINC_G4_SHADER_TYPE_VERTEX);
			load_shader("baseSprite.frag", &basicFragmentShader,KINC_G4_SHADER_TYPE_FRAGMENT);
			//-----------------------------------------------------------------------------------
			pipe.vertex_shader = &basicVertexShader;
			pipe.fragment_shader = &basicFragmentShader;
			// Create the input layout //--------------------------------------------------------
			kinc_g4_vertex_structure_init(&vertexLayout);
			kinc_g4_vertex_structure_add(&vertexLayout, "pos",	KINC_G4_VERTEX_DATA_FLOAT2);
			kinc_g4_vertex_structure_add(&vertexLayout, "tex",	KINC_G4_VERTEX_DATA_SHORT2_NORM);
			kinc_g4_vertex_structure_add(&vertexLayout, "col",	KINC_G4_VERTEX_DATA_COLOR);
			//-----------------------------------------------------------------------------------
			pipe.input_layout[0] = &vertexLayout;
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
		}

		basicPipe Pipe2d;

		namespace quad 
		{
			void init() 
			{
				init_ibuffer();
				batcher = new colored(Pipe2d);
				atlases::load_all();
			}

			void free() 
			{
				kinc_g4_index_buffer_destroy(ibuffer);
				delete ibuffer;
				delete batcher;
				atlases::free_all();
			}

			void init_ibuffer() 
			{
				ibuffer = new kinc_g4_index_buffer_t;
				kinc_g4_index_buffer_init(ibuffer, max_quads * 6, KINC_G4_INDEX_BUFFER_FORMAT_32BIT);
				auto ndx = kinc_g4_index_buffer_lock(ibuffer);
				//auto ndx = ib->lock();
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
				kinc_g4_index_buffer_unlock(ibuffer);
			}

			namespace atlases 
			{
				void free(atlas* atlas) 
				{
					delete atlas;
				}

				void load(atlas*& a, const char* name) 
				{
					a = new atlas(name, ".png");
				}

				void free_all() 
				{
					free(gui);
					//free(sprites);
				}

				void load_all() 
				{
					load(gui, "Gui");
					//load(sprites, "Sprites");
				}
			}
		}

		void init() 
		{
			Pipe2d.init();
			quad::init();
		}

		void on_resize(float w, float h)
		{
			matrices::projection(Pipe2d.spriteProjection, w / Pipe2d.scaling, h / Pipe2d.scaling);
			matrices::projection(Pipe2d.guiProjection, w, h);
		}

		void shutdown() 
		{
			quad::free();
		}
	}
}
