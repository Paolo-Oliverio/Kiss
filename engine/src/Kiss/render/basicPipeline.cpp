#include <Kiss/data/atlas.h>
#include <Kore/IO/FileReader.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/shader.h>
#include <kinc/graphics4/textureunit.h>
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

		void basicPipe::init() 
		{
			kinc_g4_pipeline_init(&pipe);
			auto shaderfile = FileReader();
			//-----------------------------------------------------------------------------------
			shaderfile.open("baseSprite.vert");
			kinc_g4_shader_init(&basicVertexShader, shaderfile.readAll(), shaderfile.size(), KINC_G4_SHADER_TYPE_VERTEX);
			pipe.vertex_shader = &basicVertexShader;
			shaderfile.close();
			//-----------------------------------------------------------------------------------
			shaderfile.open("baseSprite.frag");
			kinc_g4_shader_init(&basicFragmentShader, shaderfile.readAll(), shaderfile.size(), KINC_G4_SHADER_TYPE_FRAGMENT);
			pipe.fragment_shader = &basicFragmentShader;
			shaderfile.close();
			// Create the input layout //--------------------------------------------------------
			kinc_g4_vertex_structure_init(&vertexLayout);
			kinc_g4_vertex_structure_add(&vertexLayout, "Position", KINC_G4_VERTEX_DATA_FLOAT2);
			kinc_g4_vertex_structure_add(&vertexLayout, "UV",		KINC_G4_VERTEX_DATA_SHORT2_NORM);
			kinc_g4_vertex_structure_add(&vertexLayout, "Color",	KINC_G4_VERTEX_DATA_COLOR);
			//-----------------------------------------------------------------------------------
			pipe.input_layout[0] = &vertexLayout;
			pipe.input_layout[1] = NULL;
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

		void resize(float w, float h, float scale)
		{
			Pipe2d.spriteProjection = matrices::projection(w / scale, h / scale);
			Pipe2d.guiProjection = matrices::projection(w, h);
		}

		void shutdown() 
		{
			quad::free();
		}
	}
}
