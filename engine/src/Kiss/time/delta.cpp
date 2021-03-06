#include "delta.h"
#include "kinc/math/core.h"

namespace kiss{
	chrono time;

	float chrono::update() {
		double time = kinc_time();
		ticks += 1;
		float dt = (float)(time - lastTime);
		delta.raw = dt;
		delta.safe = kinc_min(dt, 0.333f);
		lastTime = time;
		sum += dt;
		return delta.safe;
	}
}