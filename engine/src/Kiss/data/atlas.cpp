#include "atlas.h"
#include <kinc/graphics4/texture.h>
#include <kinc/io/filereader.h>
#include <cstring>

namespace kiss 
{
	kinc_g4_texture_t* loadTexture(const char* path) {
		auto texture = new kinc_g4_texture_t;
		kinc_image_t image;
		size_t size = kinc_image_size_from_file(path);
		void* image_mem = malloc(size);
		kinc_image_init_from_file(&image, image_mem, path);
		kinc_g4_texture_init_from_image(texture, &image);
		kinc_image_destroy(&image);
		free(image_mem);
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
		//data.read(&size, sizeof(atlas::sizes));
		sprites		= new sprite[size.sprites];
		chars		= new sprite[size.chars];
		scale9s		= new scale9[size.scale9s];
		fonts		= new font[size.fonts];
		kinc_file_reader_read(af, sprites,	sizeof(sprite)	* size.sprites);
		kinc_file_reader_read(af, chars,	sizeof(sprite)	* size.chars);
		kinc_file_reader_read(af, scale9s,	sizeof(scale9)	* size.scale9s);
		kinc_file_reader_read(af, fonts,	sizeof(font)	* size.fonts);
		kinc_file_reader_close(af);
	}

	atlas::~atlas()
	{
		kinc_g4_texture_destroy(texture);
		delete texture;
		delete[] sprites;
		delete[] chars;
		delete[] scale9s;
		delete[] fonts;
	}
}
