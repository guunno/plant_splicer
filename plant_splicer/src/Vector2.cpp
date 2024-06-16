#include "Vector2.h"

#include <math.h>
#include <iostream>

float Vector2::dot(Vector2 other) const
{
	return (this->x * other.x) + (this->y * other.y);
}
float Vector2::magSq() const
{
	return (this->x * this->x) + (this->y * this->y);
}
float Vector2::mag() const
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

void Vector2::setMag(float mag)
{
	if (this->mag() == 0) return;
	this->x *= mag / this->mag();
	this->y *= mag / this->mag();
}
Vector2 Vector2::setNewMag(float mag)
{
	if (this->mag() != 0)
	{
		return Vector2(this->x, this->y) / (this->mag() * mag);
	}
	else
	{
		return { 0, 0 };
	}
}
void Vector2::limit(float maxMag)
{
	if (this->mag() > maxMag)
	{
		this->setMag(maxMag);
	}
}
Vector2 Vector2::normalize()
{
	return this->setNewMag(1);
}

Vector2 Vector2::copy() const
{
	return Vector2(this->x, this->y);
}
void Vector2::set(Vector2 other)
{
	this->x = other.x;
	this->y = other.y;
}
void Vector2::set(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vector2::heading() const
{
	if (this->y >= 0)
		return acos(this->x / this->mag());
	return -acos(this->x / this->mag());
}
void Vector2::rotate(float Angle, Vector2 Pivot)
{
	if (int(Angle * 180 / PI) % 360 == 0)
		return;

	float s = sin(Angle);
	float c = cos(Angle);

	float x = this->x - Pivot.x;
	float y = this->y - Pivot.y;

	this->x = (x * c) - (y * s) + Pivot.x;
	this->y = (x * s) + (y * c) + Pivot.y;

}
Vector2 Vector2::rotateNew(float radians, Vector2 Pivot) const
{
	Vector2 myCopy = this->copy();
	myCopy.rotate(radians, Pivot);
	return myCopy;
}
Vector2 Vector2::fromAngle(float angle)
{
	return { sin(angle), cos(angle) };
}

Vector2 Vector2::operator+(Vector2 other) const
{
	return { this->x + other.x, this->y + other.y };
}
Vector2 Vector2::operator-(Vector2 other) const
{
	return { this->x - other.x, this->y - other.y };
}
Vector2 Vector2::operator*(Vector2 other) const
{
	return { this->x * other.x, this->y * other.y };
}
Vector2 Vector2::operator/(Vector2 other) const
{
	return { this->x / other.x, this->y / other.y };
}
Vector2 Vector2::operator*(float scalar) const
{
	return { this->x * scalar, this->y * scalar };
}
Vector2 Vector2::operator/(float scalar) const
{
	return { this->x / scalar, this->y / scalar };
}

void Vector2::operator+=(Vector2 other)
{
	this->x += other.x;
	this->y += other.y;
}
void Vector2::operator-=(Vector2 other)
{
	this->x -= other.x;
	this->y -= other.y;
}
void Vector2::operator*=(Vector2 other)
{
	this->x *= other.x;
	this->y *= other.y;
}
void Vector2::operator/=(Vector2 other)
{
	this->x /= other.x;
	this->y /= other.y;
}
void Vector2::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
}
void Vector2::operator/=(float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
}

std::ostream& operator<<(std::ostream& s, const Vector2& v)
{
	s << v.x << ", " << v.y;
	return s;
}
