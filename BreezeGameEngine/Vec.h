#pragma once

#include<cmath>

template<typename T>
class Vec
{
public:
	Vec()
	{
		X = T(0);
		Y = T(0);
	}

	Vec(T x, T y)
	{
		X = x;
		Y = y;
	}

	T GetLength() const
	{
		return std::sqrt(GetLengthSq());
	}

	T GetLengthSq() const
	{
		return X * X + Y * Y;
	}

	Vec operator+(const Vec& rhs) const
	{
		return Vec(X + rhs.X, Y + rhs.Y);
	}

	Vec& operator+=(const Vec& rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		return *this;
	}

	Vec operator-(const Vec& rhs) const
	{
		return Vec(X - rhs.X, Y - rhs.Y);
	}

	Vec operator-() const
	{
		return Vec<T>(-X, -Y);
	}

	Vec& operator-=(const Vec& rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		return *this;
	}

	Vec operator*(const T scale) const
	{
		return Vec<T>(X * scale, Y * scale);
	}

	Vec operator *=(const T scale)
	{
		X *= scale;
		Y *= scale;
		return *this;
	}

	Vec operator/(const T scale) const
	{
		return Vec(X / scale, Y / scale);
	}

	Vec Norm() const
	{
		if (GetLength() != (T)0)
		{
			float mag = GetLength();
			return { X / mag, Y / mag };
		}
		else
		{
			return { 0.0f, 0.0f };
		}
	}

	T Dot(const Vec<T> rhs) const
	{
		return X * rhs.X + Y * rhs.Y;
	}

	template<typename U>
	explicit Vec(const Vec<U> src)
	{
		X = (T)src.X;
		Y = (T)src.Y;
	}

public:
	T X;
	T Y;
};

/** Could use:

typedef Vec<int> VecI;

but fuck it. **/

