#pragma once
[
#include <shape/shape.h>
#include <utility/vector2d.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

namespace Shapes {
	class Line : public Shape {
	private:
		Vector2D beginPoint;
		Vector2D endPoint;
		double thickness;

		void draw(SDL_Renderer* renderer, const Views::View* view) const noexcept override;
	public:
		Line(
			Vector2D _beginPoint,
			Vector2D _endPoint, 
			double _thickness, 
			SDL_Color color = {0, 0, 0, 255}
		) noexcept;
		void setBeginPoint(Vector2D newBeginPoint) noexcept;
		void setEndPoint(Vector2D newEndPoint) noexcept;
		void setThickness(double newThickness) noexcept;
	};
}