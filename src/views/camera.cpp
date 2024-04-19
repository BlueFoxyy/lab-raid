#include <view/camera.h>
#include <object/object.h>
#include <config.h>
#include <memory>

class Objects::Object;

namespace Views {
	Camera::Camera() : View({ 0, 0 }, { VIEW_WIDTH, VIEW_HEIGHT }) {}

	void Camera::setPosition(const Vector2D& newPosition) noexcept {
		position = newPosition;
	}

	void Camera::setPivotObject(std::shared_ptr<Objects::Object> bindObject) noexcept {
		pivotObject = bindObject;
	}

	/*
	const std::weak_ptr<Objects::Object> Camera::getPivotObject(void) const noexcept {
		return pivotObject;
	}
	*/

	Vector2D Camera::getPosition(void) const noexcept {
		if (auto pivotObjectPtr = pivotObject.lock())
			return pivotObjectPtr->getPosition();
		return position;
	}

	SDL_FRect* Camera::getRect(const Objects::Object& renderObject) const noexcept {
		SDL_FRect* retRect = new SDL_FRect();
		double sw = Config::screenWidth;
		double sh = Config::screenHeight;
		float x, y, w, h;
		Vector2D objectPosition = renderObject.getPosition();
		Vector2D objectDimension = renderObject.getDimension();
		x = objectPosition.getX(), y = objectPosition.getY();
		w = objectDimension.getX(), h = objectDimension.getY();

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Camera: Object Rect before translate: {%lf, %lf, %lf, %lf}",
			x, y, w, h
		);

		Vector2D cameraPosition = this->getPosition();
		x -= cameraPosition.getX();
		y -= cameraPosition.getY();

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Camera: Pivot object position: (%lf, %lf)",
			cameraPosition.getX(), cameraPosition.getY()
		);

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Camera: Object Rect after translate: {%lf, %lf, %lf, %lf}",
			x, y, w, h
		);

		x = x / dimension.getX() * sw;
		y = y / dimension.getY() * sh;
		w = w / dimension.getX() * sw;
		h = h / dimension.getY() * sh;
		x -= w / 2, y -= h / 2;
		x += sw / 2;
		y += sh / 2;

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Camera: Object Render Rect: {%lf, %lf, %lf, %lf}",
			x, y, w, h
		);

		return new SDL_FRect{ x, y, w, h };
	}
}