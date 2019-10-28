
#include "VecI.h"
#include<cmath>

VecI::VecI()
{
	X = 0;
	Y = 0;
}

VecI::VecI(int x, int y)
{
	X = x;
	Y = y;
}
/*
float VecI::GetLength() const
{
	return std::sqrt(GetLengthSq());
}
*/

VecI VecI::operator+(const VecI& rhs) const
{
	return VecI(X + rhs.X, Y + rhs.Y);
}

VecI& VecI::operator+=(const VecI& rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	return *this;
}

VecI VecI::operator-(const VecI& rhs) const
{
	return VecI(X - rhs.X, Y - rhs.Y);
}

VecI& VecI::operator-=(const VecI& rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	return *this;
}

VecI VecI::operator*(const int scale) const
{
	return VecI(X * scale, Y * scale);
}

VecI VecI::operator/(const int scale) const
{
	return VecI(X / scale, Y / scale);
}

/*
VecI VecI::Norm() const
{
	float mag = GetLength();
	return {X/mag, Y/mag};
}

float VecI::GetLengthSq() const
{
	return X * X + Y * Y;
}
*/
