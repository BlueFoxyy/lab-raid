#include <view/hud.h>
#include <config.h>
#include <object/object.h>

namespace Views {
	HUD::HUD() :
		View({ 0, 0 }, { VIEW_WIDTH, VIEW_HEIGHT }) {}

	SDL_FRect HUD::getRect(const Objects::Object& renderObject) const noexcept {
		Vector2D objectPosition = renderObject.getPosition();
		Vector2D objectDimension = renderObject.getDimension();
		objectPosition -= objectDimension / 2;
		objectPosition = transform(objectPosition);
		objectDimension = transform(objectDimension);

		float x, y, w, h;
		x = objectPosition.getX(), y = objectPosition.getY();
		w = objectDimension.getX(), h = objectDimension.getY();
		return SDL_FRect{ x, y, w, h };
	}

	Vector2D HUD::transform(const Vector2D& position) const noexcept {
		const Vector2D& cameraPosition = this->position;
		Vector2D relativePosition = position - cameraPosition;
		double x = relativePosition.getX();
		double y = relativePosition.getY();
		x = x / dimension.getX() * Config::screenWidth;
		y = y / dimension.getY() * Config::screenHeight;
		return { x, y };
	}
}