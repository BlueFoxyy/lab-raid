#include <object/bullet.h>

namespace Objects {
	Uint32 Bullet::getAliveTime(void) const noexcept {
		return SDL_GetTicks() - createdTick;
	}

	void Bullet::update(void) noexcept {
		this->move(Vector2D(speed, 0).rotate(this->getAngle()));
	}
};