#pragma once

#include <cmath>

class Vector2D {
private:
	float x;
	float y;
public:
// Constructors
	Vector2D(void) noexcept;
	Vector2D(float _x, float _y) noexcept;

// Member Functions
	float getX(void) const noexcept;	// x factor
	float getY(void) const noexcept;	// y factor
	Vector2D norm(void) const noexcept;	// normalized vector
	float len(void) const noexcept;		// length of vector
	float len2(void) const noexcept;	// squared length of vector
	
	void rotate(float theta) noexcept;	// rotates the vector by @param theta radians
	
// Static functions
	static Vector2D zero(void) noexcept; // returns a zero-vector
	
// Operators
	friend Vector2D operator + (const Vector2D&, const Vector2D&) noexcept;
	friend Vector2D operator - (const Vector2D&) noexcept;
	friend Vector2D operator - (const Vector2D&, const Vector2D&) noexcept;
	friend Vector2D operator * (const Vector2D&, float) noexcept;
	friend Vector2D operator / (const Vector2D&, float) noexcept;
	friend Vector2D& operator += (Vector2D&, const Vector2D&) noexcept;
	friend Vector2D& operator -= (Vector2D&, const Vector2D&) noexcept;
	friend Vector2D& operator *= (Vector2D&, float) noexcept;
	friend Vector2D& operator /= (Vector2D&, float) noexcept;
	static float dot(const Vector2D&, const Vector2D&) noexcept;
	static float cross(const Vector2D&, const Vector2D&) noexcept;
	static Vector2D rotate(Vector2D, float) noexcept;
};