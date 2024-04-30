#pragma once

#include <object/object.h>
#include <utility/vector2d.h>
#include <SDL2/SDL.h>
#include <memory>
#include <format>

namespace Objects {
	class Object;
}
namespace Views {

	/// <summary>
	/// View: defines a view area, translates the objects' virtual rects
	/// to real rendering rects.
	/// </summary>
	class View {
	protected:
		Vector2D position;
		Vector2D dimension;

		View(const Vector2D& _position, const Vector2D& _dimension) :
			position(_position), dimension(_dimension) {}
	public:
		virtual ~View() {};

		/// <summary>
		/// Gets the render rect for @param object.
		/// </summary>
		/// <param name="object">The object to be rendered.</param>
		/// <returns>The render rect of @param object.</returns>
		virtual SDL_FRect getRect(const Objects::Object& object) const noexcept = 0;

		/// <summary>
		/// Gets the transformed render position of @param position.
		/// </summary>
		/// <param name="position">The virtual position to be transformed.</param>
		/// <returns>The render position after transformation.</returns>
		virtual Vector2D transform(const Vector2D& position) const noexcept = 0;
		
		/// <summary>
		/// Gets the virtual position of @param renderPosition.
		/// </summary>
		/// <param name="renderPosition">The render position to be transformed</param>
		/// <returns>The virtual position after transformation.</returns>
		virtual Vector2D transformFromRender(const Vector2D& renderPosition) const noexcept = 0;

		/// <summary>
		/// Gets the virtual position of the view.
		/// </summary>
		/// <returns>The virtual position of the view.</returns>
		virtual Vector2D getPosition(void) const noexcept { return position; }

		/// <summary>
		/// Gets the virtual dimension of the view.
		/// </summary>
		/// <returns>The virtual dimension of the view.</returns>
		virtual Vector2D getDimension(void) const noexcept { return dimension; }

		/// <summary>
		/// Gets the rotation angle of the view.
		/// </summary>
		/// <returns>The virtual angle of the view.</returns>
		virtual float getAngle(void) const noexcept { return 0.0f; }

		/// <summary>
		/// Gets the zoom level of the view.
		/// </summary>
		/// <returns>The zoom level of the view.</returns>
		virtual float getZoom(void) const noexcept { return 1.0f; }
	};

	const int INIT_VIEW_WIDTH = 1600;
	const int INIT_VIEW_HEIGHT = 900;
}