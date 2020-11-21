#include <kinc/pch.h>
#include <inttypes.h>
//#include <cute_math2d.h>
//#include <Kore/Graphics4/Graphics.h>
//#include <kinc/graphics4/graphics.h>
//#include <Kore/Log.h>
//--to ./build_html5

//namespace gx = Kore::Graphics4;

#define FWD_KINC(x) struct kinc_##x; typedef kinc_##x  kinc_##x##_t
#define FWD_G4(x) struct kinc_g4_##x; typedef kinc_g4_##x  kinc_g4_##x##_t
/*
namespace kiss {
	typedef int64_t		s64;
	typedef uint64_t	u64;
	typedef int32_t		s32;
	typedef uint32_t	u32;
	typedef int16_t		s16;
	typedef uint16_t	u16;
	typedef int8_t		s8;
	typedef uint8_t		u8;
	typedef float		f32;
}*/

namespace kiss
{
	using u8 =	uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;   

    using s8 =	int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s64 = int64_t;

	using f32 = float;
    using f64 = double;
}