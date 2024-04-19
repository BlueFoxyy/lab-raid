#pragma once

#include "view.h"
#include <SDL.h>
#include <object/object.h>
#include <geometry/vector2d.h>
#include <memory>

class Objects::Object;
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

		SDL_FRect* getRect(const Objects::Object&) const noexcept override;
	};
}