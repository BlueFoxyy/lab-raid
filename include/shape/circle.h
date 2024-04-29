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
		float radius;
	public:
		Circle(
			const Vector2D& center,
			float radius,
			SDL_Color color = { 0, 0, 0, 255 }
		) noexcept;
		void setCenter(const Vector2D& newCenter) noexcept;
		void setRadius(float newRadius) noexcept;
	};

	class HollowCircle : public Circle {
	private:
		static const int renderEdges = 36;

		void draw(SDL_Renderer* renderer, const Views::View* view) const noexcept override;
	protected:
		uint8_t thickness;
	public:
		HollowCircle(
			uint8_t thickness,
			const Vector2D& center,
			float radius,
			SDL_Color color = { 0, 0, 0, 255 }
		) noexcept;
		void setThickness(uint8_t newThickness) noexcept;
	};
}