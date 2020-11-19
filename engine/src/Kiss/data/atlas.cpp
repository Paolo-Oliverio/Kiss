#include <Kiss/pch.h>
#include <kinc/graphics4/texture.h>
#include <kinc/io/filereader.h>
#include <cstring>
#include "atlas.h"
#include <malloc.h>

#define SAFE_DELETE(x) if(x) delete x;
#define SAFE_DELETE_ARRAY(x) if(x) delete[] x;

namespace kiss 
{
	kinc_g4_texture_t* loadTexture(const char* path) {
		kinc_g4_texture_t* texture = nullptr;
		size_t size = kinc_image_size_from_file(path);
		if (size) {
			kinc_image_t image;
			void* image_mem = alloca(size); //TODO CUSTOM ALLOCATION tmp.
			if (kinc_image_init_from_file(&image, image_mem, path)) {
				texture = new kinc_g4_texture_t; //TODO CUSTOM ALLOCATION stack.
				kinc_g4_texture_init_from_image(texture, &image);
				kinc_image_destroy(&image);
			}
			//free(image_mem);
		}
		return texture;
	}

	atlas::atlas(const char * name, const char* format)
	{
		kinc_file_reader_t atlasfile;
		auto af = &atlasfile;
		{
			char str[1024];
			strcpy(str, "assets/");
			strcat(str, name);
			//take len at name to truncate extension
			auto l = strlen(str); 
			strcat(str, format);
			//texture = new Kore::Graphics4::Texture(str);
			texture = loadTexture(str);
			//remove .atlas extension using l
			str[l] = 0;
			strcat(str, ".atlas");
			kinc_file_reader_open(af, str, KINC_FILE_TYPE_ASSET);
		}

	#ifdef NDEBUG
		atlas::sizes size;
	#endif
		kinc_file_reader_read(af, &size, sizeof(atlas::sizes));
		const size_t spritemem		= (sizeof(sprite)	* size.sprites);
		const size_t charsmem		= (sizeof(sprite)	* size.chars);
		const size_t scale9smem		= (sizeof(scale9)	* size.scale9s);
		const size_t fontsmem		= (sizeof(font)		* size.fonts);
		const size_t total = spritemem + charsmem + scale9smem + fontsmem;
		//TODO ASSERT
		char * memptr = (char*)malloc(total);
		kinc_file_reader_read(af, memptr, total);
		kinc_file_reader_close(af);
		sprites = (sprite*) memptr;
		memptr += spritemem;

#define AssignPointer(x,t) if (##x##mem) { x = (##t##*)memptr; memptr += x##mem; } else x = nullptr;
		AssignPointer(chars, sprite)
		AssignPointer(scale9s, scale9)
		AssignPointer(fonts, font)
#undef AssignPointer
	}

	void atlas::release()
	{
		kinc_g4_texture_destroy(texture);	
		delete texture;
		free(sprites);
		memset(this, 0, sizeof(this));
	}

	atlas::~atlas()
	{
		//TODO assert resources has not been released before.
	}
}