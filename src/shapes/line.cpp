#include <shape/line.h>
#include <object/object.h>
#include <view/view.h>
#include <renderer.h>

namespace Shapes {
	Line::Line(
		Vector2D _beginPoint,
		Vector2D _endPoint, 
		double _thickness,
		SDL_Color color
	) noexcept :
		beginPoint(_beginPoint), 
		endPoint(_endPoint), 
		thickness(_thickness),
		Shape(color) {}

	void Line::setBeginPoint(Vector2D newBeginPoint) noexcept {
		beginPoint = newBeginPoint;
	}

	void Line::setEndPoint(Vector2D newEndPoint) noexcept {
		endPoint = newEndPoint;
	}

	void Line::setThickness(double newThickness) noexcept {
		thickness = newThickness;
	}

	void Line::draw(SDL_Renderer* renderer, const Views::View* view) const noexcept {
		Vector2D renderBeginPoint = view->transform(beginPoint);
		Vector2D renderEndPoint = view->transform(endPoint);
		thickLineRGBA(
			renderer,
			renderBeginPoint.getX(), renderBeginPoint.getY(),
			renderEndPoint.getX(), renderEndPoint.getY(),
			thickness, color.r, color.g, color.b, color.a
		);
	}
}