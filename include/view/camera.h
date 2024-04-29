#pragma once

#include <utility/utility.h>
#include "view.h"
#include <stdexcept>
#include <format>

namespace Views {

	/// <summary>
	/// Camera for following object or stationary view.
	/// </summary>
	class Camera : public View {
	private:
		std::weak_ptr<Objects::Object> pivotObject;

		float zoom;
		float angle;

		Vector2D getPosition(void) const noexcept;
	public:
		Camera();

		/// <summary>
		/// Sets the pivot object of the camera.
		/// </summary>
		/// <param name="pivotObject">The object to pivot on.</param>
		void setPivotObject(std::shared_ptr<Objects::Object> pivotObject) noexcept;
//		const std::weak_ptr<Objects::Object> getPivotObject(void) const noexcept;
		
		/// <summary>
		/// Sets the position of the camera.
		/// </summary>
		/// <param name="newPosition">The new positions of the camera.</param>
		void setPosition(const Vector2D& newPosition) noexcept;

		/// <summary>
		/// Sets the dimensions of the camera. The new dimension vector should be positive in both components.
		/// Throws std::invalid_argument if the new dimension vector is invalid.
		/// </summary>
		/// <param name="newDimension">The new dimensions of the camera.</param>
		void setDimension(const Vector2D& newDimension);

		/// <summary>
		/// Sets the zoom level of the camera. @param zoom should be positive.
		/// Throws std::invalid_argument if @param zoom is invalid.
		/// </summary>
		/// <param name="zoom">The zoom level to be set.</param>
		void setZoom(float zoom);

		float getZoom(void) const noexcept override;

		/// <summary>
		/// Sets the rotation angle of the camera.
		/// </summary>
		/// <param name="angle">The rotation angle to be set.</param>
		void setAngle(float angle) noexcept;

		/// <summary>
		/// Rotates the view by @diffAngle
		/// </summary>
		/// <param name="diffAngle">The angle to rotate by.</param>
		void rotate(float diffAngle) noexcept;

		/// <summary>
		/// Gets the rotation angle of the camera.
		/// </summary>
		/// <returns>The rotation angle of the camera.</returns>
		float getAngle(void) const noexcept override;

		SDL_FRect getRect(const Objects::Object& object) const noexcept override;
		Vector2D transform(const Vector2D& position) const noexcept override;
	};
}