#pragma once

class Vec
{
public:
	Vec();
	Vec(float x0, float y0);
	float GetLength() const;
	float GetLengthSq() const;
	Vec operator+(const Vec& rhs) const;
	Vec& operator+=(const Vec& rhs);
	Vec operator-(const Vec& rhs) const;
	Vec& operator-=(const Vec& rhs);
	Vec operator*(const float scale) const;
	Vec Norm() const;

public:
	float X;
	float Y;
};