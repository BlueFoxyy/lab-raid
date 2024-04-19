#pragma once

#include <SDL.h>
#include <object/object.h>
#include <geometry/vector2d.h>
#include <memory>

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
		virtual ~View() = default;
		virtual SDL_FRect* getRect(const Objects::Object&) const noexcept = 0;
	};

	const int VIEW_WIDTH = 1600;
	const int VIEW_HEIGHT = 900;
}