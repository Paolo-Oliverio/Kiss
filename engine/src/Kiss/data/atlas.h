#pragma once 
#include <Kiss/pch.h>

FwdStruct(kinc_g4_texture);

namespace kiss {
	namespace tile {
		struct pos{
			s16 x;
			s16 y;
		};

		struct uv {
			s16 u;
			s16 v;
		};

		struct uvRect {
			uv min;
			uv max;
		};

		struct posRect {
			pos min;
			pos max;
		};	
	}

	struct atlas {
		struct sprite {
			tile::posRect P;
			tile::uvRect T;
		};

		struct sizes {
			u32 sprites;	//u16
			u32 chars;	//u16
			u32 scale9s;	//u16:12
			u32 fonts;	//u16:4
		};

		struct font {
			u16	firstChar;
			u16	lastChar;
			u8	start;
			u8	lineHeight;
			u8	space;
			u8	kerning;
		};
			
		struct scale9 {
			struct offset {
				u8 left;
				u8 right;
				u8 up;
				u8 down;
			};
			tile::uvRect t;
			offset offset;
		};

		//Kore::Graphics4::Texture*	texture;
		kinc_g4_texture_t*			texture;
		sprite*						sprites;
		sprite*						chars;
		scale9*						scale9s;
		font*						fonts;

		#ifndef NDEBUG
			sizes			size;
		#endif	

		atlas(const char * atlasPath, const char* textureExtension);
		~atlas();
	};
}