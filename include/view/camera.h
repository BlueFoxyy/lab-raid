#pragma once

#include "view.h"

namespace Views {

	/// <summary>
	/// Camera for following object or stationary view.
	/// </summary>
	class Camera : public View {
	private:
		std::weak_ptr<Objects::Object> pivotObject;

		Vector2D getPosition(void) const noexcept;
	public:
		Camera();
		void setPosition(const Vector2D& newPosition) noexcept;
		void setPivotObject(std::shared_ptr<Objects::Object> bindObject) noexcept;
//		const std::weak_ptr<Objects::Object> getPivotObject(void) const noexcept;

		SDL_FRect getRect(const Objects::Object& object) const noexcept override;
		Vector2D transform(const Vector2D& position) const noexcept override;
	};
}