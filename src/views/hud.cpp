#include <view/hud.h>
#include <config.h>
#include <object/object.h>

namespace Views {
	HUD::HUD() :
		View({ 0, 0 }, { VIEW_WIDTH, VIEW_HEIGHT }) {}

	SDL_FRect* HUD::getRect(const Objects::Object& renderObject) const noexcept {
		SDL_FRect* retRect = new SDL_FRect();
		double sw = Config::screenWidth;
		double sh = Config::screenHeight;
		float x, y, w, h;
		Vector2D objectPosition = renderObject.getPosition();
		Vector2D objectDimension = renderObject.getDimension();
		x = objectPosition.getX(), y = objectPosition.getY();
		w = objectDimension.getX(), h = objectDimension.getY();
		x = x / dimension.getX() * sw;
		y = y / dimension.getY() * sh;
		w = w / dimension.getX() * sw;
		h = h / dimension.getY() * sh;
		x -= w / 2, y -= h / 2;
		return new SDL_FRect{ x, y, w, h };
	}
}