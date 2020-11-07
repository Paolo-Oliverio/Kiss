#include "basicPipeline.h"
#include "quadbatcher.h"
#include <Kiss/data/atlas.h>
#include <Kore/IO/FileReader.h>
#include <cstdio>


using namespace Kore;

namespace kiss {
	namespace gfx2d {
		namespace vertex {
			namespace structure {
				gx::VertexStructure pos_uv_color;
			}
		}

		namespace quad {
			gx::IndexBuffer* ibuffer = nullptr;
			gx::PipelineState* pipe = nullptr;
			colored* batcher = nullptr;
			colored* batcher2 = nullptr;


			void init_ibuffer();
			void init_pipe();
			void init();
			void free();

			namespace atlases {
				atlas* gui = nullptr;
				atlas* sprites = nullptr;
				void free(atlas* atlas);
				void load(atlas*& a, const char* name);
				void free_all();
				void load_all();
			}
		}

		namespace matrices {
			Kore::mat3 proj;// updated from the camera on set
		}

		namespace screen {
			gx::PipelineState* pipe = nullptr;
			gx::VertexBuffer* vbuffer = nullptr;
			void init_pipe(gx::VertexStructure& structure);
			void init_vbuffer(gx::VertexStructure& structure);
			void init();
			void free();
		}

		namespace quad {
			void init() {
				init_ibuffer();
				init_pipe();
				batcher = new colored(*pipe);
				atlases::load_all();
			}

			void free() {
				delete ibuffer;
				delete pipe;
				delete batcher;
				
				atlases::free_all();
			}

			void init_ibuffer() {
				auto ib = new gx::IndexBuffer(max_quads * 6);
				auto ndx = ib->lock();
				for (int i = 0; i < max_quads; ++i) {
					ndx[0] = i * 4;
					ndx[1] = i * 4 + 1;
					ndx[2] = i * 4 + 2;
					ndx[3] = i * 4 + 1;
					ndx[4] = i * 4 + 3;
					ndx[5] = i * 4 + 2;
					ndx += 6;
				}ib->unlock();
				ibuffer = ib;
			}

			void init_pipe() {
				//Pipeline for Quad sprites rendering.
				pipe = new gx::PipelineState;
				//-----------------------------------------------------------------------------------
				FileReader vs("baseSprite.vert");
				pipe->vertexShader = new gx::Shader(vs.readAll(), vs.size(), gx::VertexShader);
				FileReader fs("baseSprite.frag");
				pipe->fragmentShader = new gx::Shader(fs.readAll(), fs.size(), gx::FragmentShader);
				//-----------------------------------------------------------------------------------
				gx::VertexStructure& structure = vertex::structure::pos_uv_color;
				structure.add("pos", gx::Float2VertexData);
				structure.add("tex", gx::Short2NormVertexData);
				structure.add("col", gx::ColorVertexData);
				pipe->inputLayout[0] = &structure;
				pipe->inputLayout[1] = NULL;
				//-----------------------------------------------------------------------------------
				pipe->blendSource = gx::SourceAlpha;
				pipe->blendDestination = gx::InverseSourceAlpha;
				pipe->alphaBlendSource = gx::SourceAlpha;
				pipe->alphaBlendDestination = gx::InverseSourceAlpha;
				//-----------------------------------------------------------------------------------
				pipe->compile();
			}

			namespace atlases {

				void free(atlas* atlas) {
					if (atlas) {
						if (atlas->texture)delete atlas->texture;
						delete atlas;
					}
				}

				void load(atlas*& a, const char* name) {
					char buffer[256];
					std::sprintf(buffer, "assets/%s.png", name);
					auto texture = new gx::Texture(buffer);
					std::sprintf(buffer, "assets/%s.atlas", name);
					a = new atlas(buffer, texture);
				}

				void free_all() {
					free(gui);
					//free(sprites);
				}

				void load_all() {
					load(gui, "Gui");
					//load(sprites, "Sprites");
				}
			}
		}

		namespace screen {
			void init_pipe(gx::VertexStructure& structure) {
				//Pipeline for Quad sprites rendering.
				pipe = new gx::PipelineState;
				//-----------------------------------------------------------------------------------
				FileReader vs("baseScreen.vert");
				pipe->vertexShader = new gx::Shader(vs.readAll(), vs.size(), gx::VertexShader);
				FileReader fs("baseScreen.frag");
				pipe->fragmentShader = new gx::Shader(fs.readAll(), fs.size(), gx::FragmentShader);
				//-----------------------------------------------------------------------------------
				pipe->inputLayout[0] = &structure;
				//-----------------------------------------------------------------------------------
				pipe->compile();
			}

			void init_vbuffer(gx::VertexStructure& structure) {
				auto vb = new gx::VertexBuffer(4, structure);
				float* ndx = (float*)vb->lock();
				float vtx[8] = {
					0, 1,
					1, 1,
					0, 0,
					1, 0
				};
				memcpy(ndx, vtx, sizeof(float) * 8);
				vb->unlock();
				vbuffer = vb;
			}

			void init() {
				gx::VertexStructure structure;
				structure.add("pos", gx::Float2VertexData);
				init_pipe(structure);
				init_vbuffer(structure);
			}

			void free() {
				delete pipe;
				delete vbuffer;
			}
		}

		void init(float width, float height) {
			matrices::projection(width, height);
			quad::init();
			//screen::init();
		}

		void shutdown() {
			quad::free();
			//screen::free();
		};
	}
}
