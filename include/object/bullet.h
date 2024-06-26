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
		const static Vector2D bulletSize;
		Bullet(
			const Views::View* view,
			Vector2D position,
			float angle,
			float speed = 20.0f
		) : Object(
				std::vector<std::string> {"bullet"},
				view,
				position,
				bulletSize
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