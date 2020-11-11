#pragma once
#include <Kiss/pch.h>

namespace kiss {



	union iColor {	
		enum : u32 {
			Black = 0xFF000000,
			White = 0xFFFFFFFF,
			Red = 0xFF0000FF,
			Blue = 0xFFFF0000,
			Green = 0xFF00FF00,
			Magenta = 0xFFFF00FF,
			Yellow = 0xFF00FFFF,
			Cyan = 0xFFFFFF00,
			Purple = 0xFF800080,
			Pink = 0xFFCBC0FF,
			Orange = 0xFF00A5FF,
			Transparent = 0x00000000,
		};

		u32 c;
		struct rgba {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} rgba;

		iColor(const u32 col) : c(col){};
		operator const u32() { return c; };
		operator const u32() const { return c; };

		iColor(u8 r, u8 g, u8 b, u8 a = 255): rgba{
			r, g, b, a
		}{}

		iColor(const f32 r, const f32 g, const f32 b, const f32 a = 1.f): rgba{
			u8(r * 255.f),
			u8(g * 255.f),
			u8(b * 255.f),
			u8(a * 255.f)
		}{}

		//FromHsv: Init from f32 hsv in the range 0-1.
		static iColor FromHsv(const u8 h, const u8 s, const f32 v, const f32 a = 1.f)
		{
			if (s <= 0.0) {       // < is bogus, just shuts up warnings
				u8 vi = u8(v * 255.f);
				return iColor(vi, vi, vi,int(a * 255.f));
			}
			const f32 hh = h * 6.f;
			const int	i = (int)hh;
			const f32 ff = hh - i;
			const f32 p = v * (1.f - s);
			const f32 q = v * (1.f - (s * ff));
			const f32 t = v * (1.f - (s * (1.f - ff)));
			switch (i) {
			case 0:
				return iColor(v, t, p, a);
			case 1:
				return iColor(q, v, p, a);
			case 2:
				return iColor(p, v, t, a);
			case 3:
				return iColor(p, q, v, a);
			case 4:
				return iColor(t, p, v, a);
			case 5:
			default:
				return iColor(v, p, q, a);
			}
		}
	};
	/*
	struct fColor {
		f32 r;
		f32 g;
		f32 b;
		f32 a;
		fColor(f32 r, f32 g, f32 b, f32 a) :r(r), g(g), b(b), a(a) {}
		fColor(Color c):
			r(c.rgba.r / 256.f),
			g(c.rgba.g / 256.f),
			b(c.rgba.b / 256.f),
			a(c.rgba.a / 256.f) {}
	};*/
}

