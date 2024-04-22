#include <shape/circle.h>

namespace Shapes {
	Circle::Circle(
		const Vector2D& center,
		double radius,
		SDL_Color color
	) noexcept :
		center(center),
		radius(radius),
		Shape(color) {}

	void Circle::setCenter(const Vector2D& newCenter) noexcept {
		center = newCenter;
	}
	
	void Circle::setRadius(double newRadius) noexcept {
		radius = newRadius;
	}

	void Circle::draw(SDL_Renderer* renderer, const Views::View* view) const noexcept {
		Vector2D renderCenter = view->transform(center);
		filledCircleRGBA(
			renderer, renderCenter.getX(), renderCenter.getY(), radius,
			color.r, color.g, color.b, color.a
		);
	}

	HollowCircle::HollowCircle(
		double thickness,
		const Vector2D& center,
		double radius,
		SDL_Color color
	) noexcept:
		thickness(thickness),
		Circle(center, radius, color) {}

	void HollowCircle::setThickness(double newThickness) noexcept {
		thickness = newThickness;
	}

	void HollowCircle::draw(SDL_Renderer* renderer, const Views::View* view) const noexcept {
		Vector2D renderCenter = view->transform(center);
		auto polarToCartesian = [](double radius, double theta) {
			return Vector2D{ radius * cos(theta), radius * sin(theta) };
		};
		Vector2D prev{ 0, radius };
		Vector2D cur;
		for (int i = 1; i <= HollowCircle::renderEdges; i++) {
			cur = polarToCartesian(radius, 2 * M_PI / i);
			thickLineRGBA(
				renderer,
				prev.getX(), prev.getY(),
				cur.getX(), cur.getY(),
				thickness,
				color.r, color.g, color.b, color.a
			);
			prev = cur;
		}
	}
}