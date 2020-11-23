#ifdef KISS_SOLOUD 
#include <Kiss/pch.h>
#include "sound.h"
#include <soloud.h>

namespace kiss
{
	namespace sound 
	{
		SoLoud::Soloud manager;

		void init() 
		{
			//TODO in html5 setup input to unlock sound first.
			manager.init(1, 0, 0, 2048, 2);
		}
		
		void release() 
		{
			manager.deinit();
		}
	}
}
#endif 