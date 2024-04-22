#pragma once

#include <shape/shape.h>
#include <utility/vector2d.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

namespace Views {
	class View;
};

namespace Shapes {
	class Circle : public Shape {
	private:
		void draw(SDL_Renderer* renderer, const Views::View* view) const noexcept override;
	protected:
		Vector2D center;
		double radius;
	public:
		Circle(
			const Vector2D& center,
			double radius,
			SDL_Color color = { 0, 0, 0, 255 }
		) noexcept;
		void setCenter(const Vector2D& newCenter) noexcept;
		void setRadius(double newRadius) noexcept;
	};

	class HollowCircle : public Circle {
	private:
		static const int renderEdges = 36;

		void draw(SDL_Renderer* renderer, const Views::View* view) const noexcept override;
	protected:
		double thickness;
	public:
		HollowCircle(
			double thickness,
			const Vector2D& center,
			double radius,
			SDL_Color color = { 0, 0, 0, 255 }
		) noexcept;
		void setThickness(double newThickness) noexcept;
	};
}