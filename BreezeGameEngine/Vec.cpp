
#include "Vec.h"
#include<cmath>

Vec::Vec()
{
}

Vec::Vec(float x, float y)
{
	X = x;
	Y = y;
}

float Vec::GetLength() const
{
	return std::sqrt(GetLengthSq());
}

Vec Vec::operator+(const Vec& rhs) const
{
	return Vec(X + rhs.X, Y + rhs.Y);
}

Vec& Vec::operator+=(const Vec& rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	return *this;
}

Vec Vec::operator-(const Vec& rhs) const
{
	return Vec(X - rhs.X, Y - rhs.Y);
}

Vec& Vec::operator-=(const Vec& rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	return *this;
}

Vec Vec::operator*(const float scale) const
{
	return Vec(X * scale, Y * scale);
}

Vec Vec::Norm() const
{
	float mag = GetLength();
	return {X/mag, Y/mag};
}

float Vec::GetLengthSq() const
{
	return X * X + Y * Y;
}