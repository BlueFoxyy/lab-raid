#pragma once

#include "vector2d.h"

class Line {
private:
	Vector2D beginPoint;
	Vector2D endPoint;
	double thickness;
public:
	Line(Vector2D _beginPoint, Vector2D _endPoint, double _thickness) noexcept;
	void setBeginPoint(Vector2D newBeginPoint) noexcept;
	void setEndPoint(Vector2D newEndPoint) noexcept;
	void setThickness(double newThickness) noexcept;
};