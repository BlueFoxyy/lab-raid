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

	SDL_FRect Camera::getRect(const Objects::Object& renderObject) const noexcept {
		double sw = Config::screenWidth;
		double sh = Config::screenHeight;
		Vector2D objectPosition = renderObject.getPosition();
		Vector2D objectDimension = renderObject.getDimension();
		Vector2D renderPosition = transform(objectPosition - objectDimension / 2);
		float x = renderPosition.getX();
		float y = renderPosition.getY();
		float w = objectDimension.getX();
		float h = objectDimension.getY();

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Camera: Object Rect before translate: {%lf, %lf, %lf, %lf}",
			x, y, w, h
		);

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Camera: Object Rect after translate: {%lf, %lf, %lf, %lf}",
			x, y, w, h
		);

		w = w / dimension.getX() * sw;
		h = h / dimension.getY() * sh;

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Camera: Object Render Rect: {%lf, %lf, %lf, %lf}",
			x, y, w, h
		);

		return SDL_FRect{ x, y, w, h };
	}

	Vector2D Camera::transform(const Vector2D& position) const noexcept {
		const Vector2D& cameraPosition = this->getPosition();
		Vector2D relativePosition = position - cameraPosition;
		double x = relativePosition.getX();
		double y = relativePosition.getY();
		x = x / dimension.getX() * Config::screenWidth + Config::screenWidth / 2.0;
		y = y / dimension.getY() * Config::screenHeight + Config::screenHeight / 2.0;
		return { x, y };
	}
}