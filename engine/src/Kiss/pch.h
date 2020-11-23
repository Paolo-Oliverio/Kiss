#include <kinc/pch.h>
#include <inttypes.h>

#define FWD_KINC(x) struct kinc_##x; typedef kinc_##x  kinc_##x##_t
#define FWD_G4(x) struct kinc_g4_##x; typedef kinc_g4_##x  kinc_g4_##x##_t

#define SAFE_DELETE(x) if(x) delete x;
#define SAFE_DELETE_ARRAY(x) if(x) delete[] x;

#ifndef KISS_IMGUI
#define WithImgui(x)
#else
#define WithImgui(x) x
#endif

#ifndef KISS_BOX2D
    #define WithBox2D(x)
#else
    #define WithBox2D(x) x
#endif

#ifndef KISS_ENTT
    #define WithEntt(x)
#else
    #define WithEntt(x) x
#endif

#ifndef KISS_SOLOUD 
    #define WithSoloud(x) 
#else 
    #define WithSoloud(x) x 
#endif

#ifndef KISS_HFSM2
    #define Withfsm(x) 
#else 
    #define Withfsm(x) x 
#endif

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