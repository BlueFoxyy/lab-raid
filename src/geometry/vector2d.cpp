#include <geometry/vector2d.h>
#include <cmath>

Vector2D::Vector2D(void) noexcept :
	x(0), y(0) {}

Vector2D::Vector2D(double _x, double _y) noexcept :
	x(_x), y(_y) {}

double Vector2D::getX(void) const noexcept {
	return x;
}

double Vector2D::getY(void) const noexcept {
	return y;
}

Vector2D Vector2D::norm(void) const noexcept {
	double l = len();
	return *this / l;
}

double Vector2D::len(void) const noexcept {
	return sqrt(len2());
}

double Vector2D::len2(void) const noexcept {
	return x * x + y * y;
}

void Vector2D::rotate(double theta) noexcept {
	double newX = x * cos(theta) - y * sin(theta);
	double newY = x * sin(theta) - y * cos(theta);
	x = newX;
	y = newY;
}

Vector2D Vector2D::zero(void) noexcept {
	return Vector2D(0, 0);
}

Vector2D operator + (const Vector2D& u, const Vector2D& v) noexcept { return Vector2D(u.x + v.x, u.y + v.y); }
Vector2D operator - (const Vector2D& u) noexcept { return Vector2D(-u.x, -u.y); }
Vector2D operator - (const Vector2D& u, const Vector2D& v) noexcept { return u + (-v); }
Vector2D operator * (const Vector2D& u, double c) noexcept { return Vector2D(u.x * c, u.y * c); }
Vector2D operator / (const Vector2D& u, double c) noexcept { return Vector2D(u.x / c, u.y / c); }
Vector2D& operator += (Vector2D& u, const Vector2D& v) noexcept { return u = u + v; }
Vector2D& operator -= (Vector2D& u, const Vector2D& v) noexcept { return u = u - v; }
Vector2D& operator *= (Vector2D& u, double c) noexcept { return u = u * c; }
Vector2D& operator /= (Vector2D& u, double c) noexcept { return u = u / c; }
double Vector2D::dot(const Vector2D& u, const Vector2D& v) noexcept {
	return u.x * v.x + u.y * v.y;
}
double Vector2D::cross(const Vector2D& u, const Vector2D& v) noexcept {
	return u.x * v.y - u.y * v.x;
}
Vector2D Vector2D::rotate(Vector2D u, double theta) noexcept {
	u.rotate(theta);
	return u;
}