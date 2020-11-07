#include "SpriteSheet.h"
#include <execution>

namespace kiss {
	namespace ecs {
		namespace system {
			//Loop behaviour returns if anim loops or stops.
			bool loops(flipbook& p, u8 pingPongTo, u8 LoopTo) {
				if (p.options.infiniteLooping || p.options.remainingLoops) {
					p.options.remainingLoops--;//branchless shit p.playingBackward = !p.PingPongLooping;//p.position = anim.numFrames - 1 + p.playingBackward * anim.numFrames;
					if (p.options.pingPongLooping) {
						p.options.playingBackward = !p.options.playingBackward;
						p.position = pingPongTo;
					}
					else {
						p.position = LoopTo;
					}
					return true;
				}
				return false;
			}

			//iterative search as for the common case one should not skip a frame.
			//return if the Player is stopped and should be removed.
			bool updatePlayer(flipbook& p, flipbookData& anim,  f32 t) {
				do {
					t += anim.duration;
					if (p.options.playingBackward) {
						if (--p.position < 0 && !loops(p, 1, anim.numFrames - 1)) {
							return true;
						}
					}
					else if (++p.position >= anim.numFrames && !loops(p, anim.numFrames - 2, 0)) {
						return true;
					}
				} while (t <= 0); 
				p.toNextFrame = t;
				return false;
			}

			void UpdateFlipbooks(entt::registry& ecs, f32 dt) {
				//s32 idt = (s32)(dt * 6000.f);
				f32 time;
				auto view = ecs.view<flipbook>();
				for (auto entity : view) {
					auto &p = view.get<flipbook>(entity);
					time = p.toNextFrame - dt * p.playbackSpeed;
					if (time >= 0) {
						p.toNextFrame = time;
					} else if (updatePlayer(p, *p.anim, time)) {
						ecs.remove<flipbook>(entity);
					}
				}
				/*std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view, dt](auto entity) {
					auto& p = view.get<flipbook>(entity);
					f32 time = p.toNextFrame - dt * p.playbackSpeed;
					if (time >= 0) {
						p.toNextFrame = time;
						//continue;
					}
					else {
						auto& anim = *p.anim;
						updatePlayer(p, anim, time);
					}
				});*/
				//update shared animations.
			}		
		}
	}
}