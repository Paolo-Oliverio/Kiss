#pragma once
#ifdef KISS_ENTT
#include <Kiss/pch.h>
#include <Kiss/components/sprite.h>
#include <Kiss/math/rot.h>
#include <Kiss/math/aabb.h>

#include "entt/entt.hpp"
#include <kinc/math/random.h>

//#include <execution>

namespace kiss {
	namespace ecs {

		struct pos2d : v2 {

			pos2d(v2 v) : v2(v) {}

			static pos2d rand(aabb& box) {
				return pos2d{
					v2(
						(f32)kinc_random_get_in((u32)box.min.x,(u32)box.max.x),
						(f32)kinc_random_get_in((u32)box.min.y,(u32)box.max.y)
					)
				};
			}
		};

		struct vel2d : v2 {

			vel2d(v2 v) : v2(v) {}

			static vel2d rand(s32 deg_min, s32 deg_max, f32 multiplier) {
				return vel2d{
					rot(deg2rad((f32)kinc_random_get_in(deg_min, deg_max))).to_v2() * multiplier
				};
			}
		};

		namespace move {

			void step(entt::registry& ecs, float dt) {
				auto view = ecs.group<pos2d, vel2d>();
				for (auto entity : view) {
					auto& pos = view.get<pos2d>(entity);
					const auto& vel = view.get<vel2d>(entity);
					pos += vel * dt;
				}
			}

			void step_in_aabb(entt::registry& ecs, const float dt, const aabb b) {
				auto view = ecs.group<pos2d, vel2d>();

				for (auto entity : view) {
					auto [pos, vel] = view.get<pos2d, vel2d>(entity);
					pos += vel * dt;
					if (pos.x < b.min.x) {
						pos.x = b.min.x;
						vel.x = -vel.x;
					}
					else if (pos.x > b.max.x) {
						pos.x = b.max.x;
						vel.x = -vel.x;
					}
					if (pos.y < b.min.y) {
						pos.y = b.min.y;
						vel.y = -vel.y;
					}
					else if (pos.y > b.max.y) {
						pos.y = b.max.y;
						vel.y = -vel.y;
					}
				}

			/*	std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view,dt,b](auto entity) {
					auto& [pos, vel] = view.get<pos2d, vel2d>(entity);
					pos += vel * dt;
					if (pos.x < b.min.x) {
						pos.x = b.min.x;
						vel.x = -vel.x;
					}
					else if (pos.x > b.max.x) {
						pos.x = b.max.x;
						vel.x = -vel.x;
					}
					if (pos.y < b.min.y) {
						pos.y = b.min.y;
						vel.y = -vel.y;
					}
					else if (pos.y > b.max.y) {
						pos.y = b.max.y;
						vel.y = -vel.y;
					}
				});*/
				
			}
		}
	}
}
#endif