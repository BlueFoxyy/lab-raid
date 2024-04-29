#include <shape/line.h>

namespace Shapes {
	Line::Line(
		Views::View* view,
		Vector2D beginPoint,
		Vector2D endPoint, 
		uint8_t thickness,
		SDL_Color color
	) noexcept :
		beginPoint(beginPoint), 
		endPoint(endPoint), 
		thickness(thickness),
		Shape(view, color) {}

	void Line::setBeginPoint(Vector2D newBeginPoint) noexcept {
		beginPoint = newBeginPoint;
	}

	void Line::setEndPoint(Vector2D newEndPoint) noexcept {
		endPoint = newEndPoint;
	}

	void Line::setThickness(uint8_t newThickness) noexcept {
		thickness = newThickness;
	}

	void Line::draw(SDL_Renderer* renderer) const noexcept {
		Vector2D renderBeginPoint = view->transform(beginPoint);
		Vector2D renderEndPoint = view->transform(endPoint);
//		SDL_LogDebug(
//			SDL_LOG_CATEGORY_APPLICATION,
//			"Line::draw(): (%lf, %lf) -> (%lf, %lf), width: %u",
//			renderBeginPoint.getX(), renderBeginPoint.getY(),
//			renderEndPoint.getX(), renderEndPoint.getY(),
//			thickness
//		);
		thickLineRGBA(
			renderer,
			static_cast<Sint16>(renderBeginPoint.getX()),
			static_cast<Sint16>(renderBeginPoint.getY()),
			static_cast<Sint16>(renderEndPoint.getX()),
			static_cast<Sint16>(renderEndPoint.getY()),
			static_cast<Uint8>(ceilf(thickness * view->getZoom())),
			color.r, color.g, color.b, color.a
		);
	}
}