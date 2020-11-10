#include "atlas.h"
#include <Kore/IO/FileReader.h>
#include <Kore/Graphics4/Texture.h>
#include <cstring>

namespace kiss 
{
	atlas::atlas(const char * name, const char* format)
	{
		auto data = Kore::FileReader();
		{
			char str[1024];
			strcpy(str, "assets/");
			strcat(str, name);
			//take len at name to truncate extension
			auto l = strlen(str); 
			strcat(str, ".atlas");
			data.open(str);
			//remove .atlas extension using l
			str[l] = 0;
			strcat(str, format);
			texture = new Kore::Graphics4::Texture(str);
		}

	#ifdef NDEBUG
		atlas::sizes size;
	#endif
		data.read(&size, sizeof(atlas::sizes));
		sprites		= new sprite[size.sprites];
		chars		= new sprite[size.chars];
		scale9s		= new scale9[size.scale9s];
		fonts		= new font[size.fonts];
		data.read(sprites,	sizeof(sprite)* size.sprites);
		data.read(chars,	sizeof(sprite)* size.chars);
		data.read(scale9s,	sizeof(scale9)* size.scale9s);
		data.read(fonts,	sizeof(font)  * size.fonts);
		data.close();
	}

	atlas::~atlas()
	{
		delete texture;
		delete[] sprites;
		delete[] chars;
		delete[] scale9s;
		delete[] fonts;
	}
}
