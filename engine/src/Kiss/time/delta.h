#pragma once
#include <Kiss/pch.h>
#include <kinc/system.h>
#include <cstring>

//TODO: smooth time
namespace kiss {
	struct chrono {
		double lastTime;
		Kore::u64 ticks = 0;
		double sum = 0;
		struct Delta {
			float raw;
			float safe;
		} delta;
		void init() { lastTime = kinc_time(); }
		float update();
	};
	extern chrono time;
}