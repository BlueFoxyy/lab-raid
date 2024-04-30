#pragma once

#include <shape/shape.h>
#include <utility/vector2d.h>
#include <utility/utility.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

namespace Views {
	class View;
};

namespace Shapes {
	class Circle : public Shape {
	protected:
		Vector2D center;
		float radius;
	public:
		Circle(
			Views::View* view,
			const Vector2D& center,
			float radius,
			SDL_Color color = { 0, 0, 0, 255 }
		) noexcept;
		void setCenter(const Vector2D& newCenter) noexcept;
		void setRadius(float newRadius) noexcept;
		void draw(SDL_Renderer* renderer) const noexcept override;
	};

	class HollowCircle : public Circle {
	private:
		static const int renderEdges = 36;
	protected:
		uint8_t thickness;
	public:
		HollowCircle(
			Views::View* view,
			uint8_t thickness,
			const Vector2D& center,
			float radius,
			SDL_Color color = { 0, 0, 0, 255 }
		) noexcept;
		void setThickness(uint8_t newThickness) noexcept;
		void draw(SDL_Renderer* renderer) const noexcept override;
	};
}