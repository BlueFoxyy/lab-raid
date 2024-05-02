#pragma once

#include "object.h"
#include <utility/vector2d.h>
#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace Objects {
	class Bullet : public Object {
	private:
		Uint32 createdTick;
		float speed;
	public:
		Bullet(
			const Views::View* view,
			Vector2D position,
			float angle,
			float speed = 7.0f
		) : Object(
				std::vector<std::string> {"bullet"},
				view,
				position,
				Vector2D{ 10, 3 }
			),
			createdTick(SDL_GetTicks()),
			speed(speed) {
			this->setAngle(angle);
		}

		/// <summary>
		/// Gets the alive time of this bullet.
		/// </summary>
		/// <returns>The alive time of this bullet.</returns>
		Uint32 getAliveTime(void) const noexcept;

		void update(void) noexcept override;
	};
}