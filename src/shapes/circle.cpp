#include <shape/circle.h>

namespace Shapes {
	Circle::Circle(
		Views::View* view,
		const Vector2D& center,
		float radius,
		SDL_Color color
	) noexcept :
		center(center),
		radius(radius),
		Shape(view, color) {}

	void Circle::setCenter(const Vector2D& newCenter) noexcept {
		center = newCenter;
	}
	
	void Circle::setRadius(float newRadius) noexcept {
		radius = newRadius;
	}

	void Circle::draw(SDL_Renderer* renderer) const noexcept {
		Vector2D renderCenter = view->transform(center);
		float renderRadius = radius * view->getZoom();
		filledCircleRGBA(
			renderer, 
			static_cast<Sint16>(renderCenter.getX()), 
			static_cast<Sint16>(renderCenter.getY()),
			static_cast<Sint16>(roundf(renderRadius) + 0.5),
			color.r, color.g, color.b, color.a
		);
	}

	HollowCircle::HollowCircle(
		Views::View* view,
		const Vector2D& center,
		float radius,
		uint8_t thickness,
		SDL_Color color
	) noexcept:
		thickness(thickness),
		Circle(view, center, radius, color) {}

	void HollowCircle::setThickness(uint8_t newThickness) noexcept {
		thickness = newThickness;
	}

	void HollowCircle::draw(SDL_Renderer* renderer) const noexcept {
		Vector2D renderCenter = view->transform(center);
		float renderRadius = radius * view->getZoom();
		Vector2D prev = renderCenter + polarToCartesian(renderRadius, view->getAngle());
		Vector2D cur;
		for (int i = 1; i <= HollowCircle::renderEdges; i++) {
			cur = renderCenter + polarToCartesian(renderRadius, view->getAngle() + static_cast<float>(2 * M_PI / renderEdges * i));
//			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
//				"HollowCircle::draw(): drawing node at (%f, %f) + (%f, %f), polar coordinates: (%f, %f).",
//				renderCenter.getX(), renderCenter.getY(),
//				(cur - renderCenter).getX(), (cur - renderCenter).getY(),
//				radius, view->getAngle() + static_cast<float>(2 * M_PI / renderEdges * i)
//			);
			thickLineRGBA(
				renderer,
				static_cast<Sint16>(prev.getX()),
				static_cast<Sint16>(prev.getY()),
				static_cast<Sint16>(cur.getX()),
				static_cast<Sint16>(cur.getY()),
				static_cast<Uint8>(ceilf(thickness * view->getZoom())),
				color.r, color.g, color.b, color.a
			);
			prev = cur;
		}
	}
}