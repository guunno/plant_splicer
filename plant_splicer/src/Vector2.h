#pragma once

#include <iostream>

#define PI 3.1415926535

class Vector2
{
public:
	friend std::ostream& operator<<(std::ostream& s, const Vector2& v);

public:
	Vector2() {}
	Vector2(float x, float y) :x(x), y(y) {}
	Vector2(const Vector2& other) :x(other.x), y(other.y) {}
	~Vector2() {}

public: // DOT PRODUCT AND MAGNITUDE MATH
	float dot(Vector2 other) const;
	float magSq() const;
	float mag() const;

public: // RESIZE VECTOR TO MAGNITUDE
	void setMag(float mag);
	Vector2 setNewMag(float mag);

	void limit(float maxMag);
	Vector2 normalize();

public: // SET/COPY VECTOR TO OTHER
	void operator=(const Vector2& other) { this->x = other.x; this->y = other.y; }
	void set(Vector2 other);
	void set(float x, float y);
	Vector2 copy() const;

public: // ANGLE MATHS
	float heading() const;
	void rotate(float Angle, Vector2 Pivot = { 0, 0 });
	Vector2 rotateNew(float radians, Vector2 Pivot = { 0, 0 }) const;
	static Vector2 fromAngle(float angle);

public: // CONST OPERATORS
	Vector2 operator+(Vector2 other) const;
	Vector2 operator-(Vector2 other) const;
	Vector2 operator*(Vector2 other) const;
	Vector2 operator/(Vector2 other) const;
	Vector2 operator*(float scalar) const;
	Vector2 operator/(float scalar) const;

public: // NON-CONST OPERATORS
	void operator+=(Vector2 other);
	void operator-=(Vector2 other);
	void operator*=(Vector2 other);
	void operator/=(Vector2 other);
	void operator*=(float scalar);
	void operator/=(float scalar);

public: // COMPARISON OPERTORS
	bool operator==(Vector2 other) const { return (other.x == this->x) && (other.y == this->y); }
	bool operator!=(Vector2 other) const { return !this->operator==(other); }

public:
	float x = 0, y = 0;
};
