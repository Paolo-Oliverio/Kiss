#ifdef KISS_SOLOUD 
#include <Kiss/pch.h>

namespace SoLoud {
	class Soloud;
}

namespace kiss
{
	namespace sound {
		extern SoLoud::Soloud manager;
		void init();
		void release();
	}
}
#endif