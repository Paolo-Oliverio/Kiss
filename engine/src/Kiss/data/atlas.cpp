#include "atlas.h"
#include <Kore/IO/FileReader.h>

namespace kiss {
	atlas::atlas(const char * path, gx::Texture * tex) : texture(tex) {
		Kore::FileReader f(path);
	#ifdef NDEBUG
		atlas::sizes size;
	#endif
		f.read(&size, sizeof(atlas::sizes));
		sprites		= new sprite[size.sprites];
		chars		= new sprite[size.chars];
		scale9s		= new scale9[size.scale9s];
		fonts		= new font[size.fonts];
		f.read(sprites,	sizeof(sprite)* size.sprites);
		f.read(chars,	sizeof(sprite)* size.chars);
		f.read(scale9s,	sizeof(scale9)* size.scale9s);
		f.read(fonts,	sizeof(font)  * size.fonts);
		f.close();
	}
}
