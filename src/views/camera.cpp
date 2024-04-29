#include <view/camera.h>

namespace Views {
	Camera::Camera() :
		View({ 0, 0 }, { INIT_VIEW_WIDTH, INIT_VIEW_HEIGHT }),
		zoom(1.0f),
		angle(0.0f) {}

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

	void Camera::setDimension(const Vector2D& newDimension) {
		if (newDimension.getX() <= 0 or newDimension.getY() <= 0)
			throw std::invalid_argument(std::format(
				"In View::setDimension(): invalid dimension vector received: ({}, {}).",
				newDimension.getX(), newDimension.getY()
			));
		dimension = newDimension;
	}

	void Camera::setZoom(float zoom) {
		if (zoom <= 0)
			throw std::invalid_argument(std::format(
				"In Camera::setZoom(): invalid zoom level received: {}.",
				zoom
			));
		this->zoom = zoom;
	}

	float Camera::getZoom(void) const noexcept {
		return zoom;
	}

	void Camera::setAngle(float angle) noexcept {
		this->angle = normalizeAngle(angle);
	}

	void Camera::rotate(float diffAngle) noexcept {
		this->angle = normalizeAngle(this->angle + diffAngle);
	}

	Vector2D Camera::getPosition(void) const noexcept {
		if (auto pivotObjectPtr = pivotObject.lock())
			return pivotObjectPtr->getPosition();
		return position;
	}

	SDL_FRect Camera::getRect(const Objects::Object& renderObject) const noexcept {
		float sw = Config::screenWidth;
		float sh = Config::screenHeight;
		Vector2D objectPosition = renderObject.getPosition();
		Vector2D objectDimension = renderObject.getDimension() * zoom;
		Vector2D renderPosition = transform(objectPosition);
		float x = renderPosition.getX();
		float y = renderPosition.getY();
		float w = objectDimension.getX();
		float h = objectDimension.getY();

		w = w / dimension.getX() * sw;
		h = h / dimension.getY() * sh;
		x -= w / 2;
		y -= h / 2;

		return SDL_FRect{ x, y, w, h };
	}

	float Camera::getAngle(void) const noexcept {
		return angle;
	}

	Vector2D Camera::transform(const Vector2D& position) const noexcept {
		const Vector2D& cameraPosition = this->getPosition();
		Vector2D relativePosition = (position - cameraPosition).rotate(-angle) * zoom;
		float x = relativePosition.getX();
		float y = relativePosition.getY();
		x = x / dimension.getX() * Config::screenWidth + Config::screenWidth / 2.0f;
		y = y / dimension.getY() * Config::screenHeight + Config::screenHeight / 2.0f;
		return { x, y };
	}
}