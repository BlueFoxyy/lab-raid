#include <shape/circle.h>

namespace Shapes {
	Circle::Circle(
		const Vector2D& center,
		float radius,
		SDL_Color color
	) noexcept :
		center(center),
		radius(radius),
		Shape(color) {}

	void Circle::setCenter(const Vector2D& newCenter) noexcept {
		center = newCenter;
	}
	
	void Circle::setRadius(float newRadius) noexcept {
		radius = newRadius;
	}

	void Circle::draw(SDL_Renderer* renderer, const Views::View* view) const noexcept {
		Vector2D renderCenter = view->transform(center);
		filledCircleRGBA(
			renderer, 
			static_cast<Sint16>(renderCenter.getX()), 
			static_cast<Sint16>(renderCenter.getY()),
			static_cast<Sint16>(roundf(radius) + 0.5),
			color.r, color.g, color.b, color.a
		);
	}

	HollowCircle::HollowCircle(
		uint8_t thickness,
		const Vector2D& center,
		float radius,
		SDL_Color color
	) noexcept:
		thickness(thickness),
		Circle(center, radius, color) {}

	void HollowCircle::setThickness(uint8_t newThickness) noexcept {
		thickness = newThickness;
	}

	void HollowCircle::draw(SDL_Renderer* renderer, const Views::View* view) const noexcept {
		Vector2D renderCenter = view->transform(center);
		auto polarToCartesian = [](float radius, float theta) {
			return Vector2D{ radius * cosf(theta), radius * sinf(theta) };
		};
		Vector2D prev{ 0, radius };
		Vector2D cur;
		for (int i = 1; i <= HollowCircle::renderEdges; i++) {
			cur = polarToCartesian(radius, static_cast<float>(2 * M_PI / i));
			thickLineRGBA(
				renderer,
				static_cast<Sint16>(prev.getX()),
				static_cast<Sint16>(prev.getY()),
				static_cast<Sint16>(cur.getX()), 
				static_cast<Sint16>(cur.getY()),
				thickness,
				color.r, color.g, color.b, color.a
			);
			prev = cur;
		}
	}
}