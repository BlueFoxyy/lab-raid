#pragma once

class Vector2D {
private:
	double x;
	double y;
public:
// Constructors
	Vector2D(void) noexcept;
	Vector2D(double _x, double _y) noexcept;

// Member Functions
	double getX(void) const noexcept;	// x factor
	double getY(void) const noexcept;	// y factor
	Vector2D norm(void) const noexcept;	// normalized vector
	double len(void) const noexcept;		// length of vector
	double len2(void) const noexcept;	// squared length of vector
	
	void rotate(double theta) noexcept;	// rotates the vector by @param theta radians
	
// Static functions
	static Vector2D zero(void) noexcept; // returns a zero-vector
	
// Operators
	friend Vector2D operator + (const Vector2D&, const Vector2D&) noexcept;
	friend Vector2D operator - (const Vector2D&) noexcept;
	friend Vector2D operator - (const Vector2D&, const Vector2D&) noexcept;
	friend Vector2D operator * (const Vector2D&, double) noexcept;
	friend Vector2D operator / (const Vector2D&, double) noexcept;
	friend Vector2D& operator += (Vector2D&, const Vector2D&) noexcept;
	friend Vector2D& operator -= (Vector2D&, const Vector2D&) noexcept;
	friend Vector2D& operator *= (Vector2D&, double) noexcept;
	friend Vector2D& operator /= (Vector2D&, double) noexcept;
	static double dot(const Vector2D&, const Vector2D&) noexcept;
	static double cross(const Vector2D&, const Vector2D&) noexcept;
	static Vector2D rotate(Vector2D, double) noexcept;
};