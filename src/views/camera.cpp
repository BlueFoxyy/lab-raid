#include <view/camera.h>

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
		float sw = Config::screenWidth;
		float sh = Config::screenHeight;
		Vector2D objectPosition = renderObject.getPosition();
		Vector2D objectDimension = renderObject.getDimension();
		Vector2D renderPosition = transform(objectPosition - objectDimension / 2);
		float x = renderPosition.getX();
		float y = renderPosition.getY();
		float w = objectDimension.getX();
		float h = objectDimension.getY();

		w = w / dimension.getX() * sw;
		h = h / dimension.getY() * sh;

		return SDL_FRect{ x, y, w, h };
	}

	Vector2D Camera::transform(const Vector2D& position) const noexcept {
		const Vector2D& cameraPosition = this->getPosition();
		Vector2D relativePosition = position - cameraPosition;
		float x = relativePosition.getX();
		float y = relativePosition.getY();
		x = x / dimension.getX() * Config::screenWidth + Config::screenWidth / 2.0f;
		y = y / dimension.getY() * Config::screenHeight + Config::screenHeight / 2.0f;
		return { x, y };
	}
}