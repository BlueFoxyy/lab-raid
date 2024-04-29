#pragma once

#include <shape/shape.h>
#include <utility/vector2d.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

namespace Shapes {
	class Line : public Shape {
	protected:
		Vector2D beginPoint;
		Vector2D endPoint;
		uint8_t thickness;
	public:
		Line(
			Views::View* view,
			Vector2D _beginPoint,
			Vector2D _endPoint, 
			uint8_t _thickness,
			SDL_Color color = {0, 0, 0, 255}
		) noexcept;
		void setBeginPoint(Vector2D newBeginPoint) noexcept;
		void setEndPoint(Vector2D newEndPoint) noexcept;
		void setThickness(uint8_t newThickness) noexcept;
		void draw(SDL_Renderer* renderer) const noexcept override;
	};
}