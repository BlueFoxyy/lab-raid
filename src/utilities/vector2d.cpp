#include <utility/vector2d.h>

Vector2D::Vector2D(void) noexcept :
	x(0), y(0) {}

Vector2D::Vector2D(float _x, float _y) noexcept :
	x(_x), y(_y) {}

float Vector2D::getX(void) const noexcept {
	return x;
}

float Vector2D::getY(void) const noexcept {
	return y;
}

Vector2D Vector2D::norm(void) const noexcept {
	float l = len();
	return *this / l;
}

float Vector2D::len(void) const noexcept {
	return sqrt(len2());
}

float Vector2D::len2(void) const noexcept {
	return x * x + y * y;
}

void Vector2D::rotate(float theta) noexcept {
	float newX = x * cos(theta) - y * sin(theta);
	float newY = x * sin(theta) - y * cos(theta);
	x = newX;
	y = newY;
}

Vector2D Vector2D::zero(void) noexcept {
	return Vector2D(0, 0);
}

Vector2D operator + (const Vector2D& u, const Vector2D& v) noexcept { return Vector2D(u.x + v.x, u.y + v.y); }
Vector2D operator - (const Vector2D& u) noexcept { return Vector2D(-u.x, -u.y); }
Vector2D operator - (const Vector2D& u, const Vector2D& v) noexcept { return u + (-v); }
Vector2D operator * (const Vector2D& u, float c) noexcept { return Vector2D(u.x * c, u.y * c); }
Vector2D operator / (const Vector2D& u, float c) noexcept { return Vector2D(u.x / c, u.y / c); }
Vector2D& operator += (Vector2D& u, const Vector2D& v) noexcept { return u = u + v; }
Vector2D& operator -= (Vector2D& u, const Vector2D& v) noexcept { return u = u - v; }
Vector2D& operator *= (Vector2D& u, float c) noexcept { return u = u * c; }
Vector2D& operator /= (Vector2D& u, float c) noexcept { return u = u / c; }
float Vector2D::dot(const Vector2D& u, const Vector2D& v) noexcept {
	return u.x * v.x + u.y * v.y;
}
float Vector2D::cross(const Vector2D& u, const Vector2D& v) noexcept {
	return u.x * v.y - u.y * v.x;
}
Vector2D Vector2D::rotate(Vector2D u, float theta) noexcept {
	u.rotate(theta);
	return u;
}