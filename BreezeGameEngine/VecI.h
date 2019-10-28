#pragma once

class VecI
{
public:
	VecI();
	VecI(int x0, int y0);
//	float GetLength() const;
//	float GetLengthSq() const;
	VecI operator+(const VecI& rhs) const;
	VecI& operator+=(const VecI& rhs);
	VecI operator-(const VecI& rhs) const;
	VecI& operator-=(const VecI& rhs);
	VecI operator*(const int scale) const;
	VecI operator/(const int scale) const;
//	VecI Norm() const;

public:
	int X;
	int Y;
};