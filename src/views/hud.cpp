#include <view/hud.h>

namespace Views {
	HUD::HUD() :
		View({ 0, 0 }, { INIT_VIEW_WIDTH, INIT_VIEW_HEIGHT }) {}

	SDL_FRect HUD::getRect(const Objects::Object& renderObject) const noexcept {
		Vector2D objectPosition = renderObject.getPosition();
		Vector2D objectDimension = renderObject.getDimension();
		
		/*
		SDL_LogDebug(
			SDL_LOG_CATEGORY_APPLICATION,
			"HUD: Object Rect before translate: {%lf, %lf, %lf, %lf}",
			objectPosition.getX(), objectPosition.getY(), 
			objectDimension.getX(), objectDimension.getY()
		);
		*/

		objectPosition = transform(objectPosition);
		objectDimension = transform(objectDimension);

		/*
		SDL_LogDebug(
			SDL_LOG_CATEGORY_APPLICATION,
			"HUD: Object Rect after translate: {%lf, %lf, %lf, %lf}",
			objectPosition.getX(), objectPosition.getY(),
			objectDimension.getX(), objectDimension.getY()
		);
		*/

		float x, y, w, h;
		x = objectPosition.getX(), y = objectPosition.getY();
		w = objectDimension.getX(), h = objectDimension.getY();
		return SDL_FRect{ x, y, w, h };
	}

	Vector2D HUD::transform(const Vector2D& position) const noexcept {
		const Vector2D& cameraPosition = this->position;
		Vector2D relativePosition = position - cameraPosition;
		float x = relativePosition.getX();
		float y = relativePosition.getY();
		x = x / dimension.getX() * Config::screenWidth;
		y = y / dimension.getY() * Config::screenHeight;
		return { x, y };
	}
}