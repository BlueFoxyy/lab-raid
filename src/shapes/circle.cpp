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

	// TODO: fix circle rendering at different aspect ratio and camera rotation
	void Circle::draw(SDL_Renderer* renderer) const noexcept {
		Vector2D renderCenter = view->transform(center);
		Vector2D renderArc = view->transform(center + Vector2D{radius, 0});
		float renderRadius = (renderArc - renderCenter).len();
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
		Vector2D prev = center + Vector2D{radius, 0};
		Vector2D renderPrev = view->transform(prev);
		Vector2D cur;
		Vector2D renderCur;
		for (int i = 1; i <= HollowCircle::renderEdges; i++) {
			cur = center + polarToCartesian(radius, static_cast<float>(2 * M_PI / renderEdges * i));
//			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
//				"HollowCircle::draw(): drawing node at (%f, %f) + (%f, %f), polar coordinates: (%f, %f).",
//				renderCenter.getX(), renderCenter.getY(),
//				(cur - renderCenter).getX(), (cur - renderCenter).getY(),
//				radius, view->getAngle() + static_cast<float>(2 * M_PI / renderEdges * i)
//			);
			auto renderCur = view->transform(cur);
			thickLineRGBA(
				renderer,
				static_cast<Sint16>(renderPrev.getX()),
				static_cast<Sint16>(renderPrev.getY()),
				static_cast<Sint16>(renderCur.getX()),
				static_cast<Sint16>(renderCur.getY()),
				static_cast<Uint8>(ceilf(thickness * view->getZoom())),
				color.r, color.g, color.b, color.a
			);
			prev = cur;
			renderPrev = renderCur;
		}
	}
}