#pragma once
#ifdef KISS_ENTT
#include <Kiss/pch.h>
#include <entt/entt.hpp>

namespace kiss {
	namespace ecs {
		struct sprite {
			u16 id;
			u8	rotation : 4; //16 rotations 22.5 degree snap
			u8	flipx : 1;
			u8	flipy : 1;
			//TODO free data
			//u8	Shared  : 1;?
			//u8	Free	: 1;?
		};

		struct flipbookData {
			f32 duration;
			u16 baseFrame;//Sprite id
			u8	numFrames;
			//TODO free data
		};

		struct flipbook {
			struct properties {
				u8	infiniteLooping : 1;
				u8	playingBackward : 1;
				u8	pingPongLooping : 1;
				u8  remainingLoops : 4;//max 16 times.
				u8	stopped : 1;
			};
			flipbookData* anim;
			f32 playbackSpeed;
			f32 toNextFrame;//second == 6000;
			//add_sprite id in atlas;
			s8	frameOffset = -1;//offset from add_sprite id.
			properties options;
		
			//flipook() { options.stopped = true; };//defaults to stopped player.
		private : 
			flipbook(flipbookData* anim, float frame, properties options, f32 speed) : anim(anim), toNextFrame(), options(options), playbackSpeed(speed){}
		
		public:
			inline u16 getFrame() const {
				return anim->baseFrame + frameOffset;
			}

			static inline void play(entt::registry& ecs, entt::entity entity, flipbookData* anim, flipbook::properties options, f32 startFrame = 0.f , f32 speed = 1.f) {
				auto p = flipbook(anim, - (startFrame * anim->duration), options, speed);
				ecs.emplace_or_replace<flipbook>(entity, p);
			}
		};
	}
}
#endif