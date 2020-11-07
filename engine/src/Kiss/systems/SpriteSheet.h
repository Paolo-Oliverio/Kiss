#pragma once
#include <Kiss/pch.h>
#include <Kiss/components/sprite.h>
#include <entt/entt.hpp>

namespace kiss {
	namespace ecs {
			constexpr int SharedPlayersNumber = 8;
			namespace system {
				void UpdateFlipbooks(entt::registry& ecs, float dt);
			}
			
			namespace spriteAnimation {
				
			}
	}
}