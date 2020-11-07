#include "delta.h"

namespace kiss{
	chrono time;

	float chrono::update() {
		double time = Kore::System::time();
		ticks += 1;
		float dt = (float)(time - lastTime);
		delta.raw = dt;
		delta.safe = Kore::min(Kore::max(dt, 0.0001f), 0.333f);
		lastTime = time;
		sum += dt;
		return delta.safe;
	}
}