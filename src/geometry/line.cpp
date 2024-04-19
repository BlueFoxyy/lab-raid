#include <geometry/line.h>
#include <object/object.h>

Line::Line(Vector2D _beginPoint, Vector2D _endPoint, double _thickness) noexcept :
	beginPoint(_beginPoint), endPoint(_endPoint), thickness(_thickness) {}

void Line::setBeginPoint(Vector2D newBeginPoint) noexcept {
	beginPoint = newBeginPoint;
}

void Line::setEndPoint(Vector2D newEndPoint) noexcept {
	endPoint = newEndPoint;
}

void Line::setThickness(double newThickness) noexcept {
	thickness = newThickness;
}