/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#pragma once

#include "Vec.h"
#include <math.h>

template<typename T>
class Rect
{
public:
	Rect() = default;
	Rect(T x0, T y0, T x1, T y1)
	{
		X0 = x0;
		Y0 = y0;
		X1 = x1;
		Y1 = y1;

		if (X0 > X1)
		{
			T temp = X1;
			X1 = X0;
			X0 = temp;
		}

		if (Y0 > Y1)
		{
			T temp = Y1;
			Y1 = Y0;
			Y0 = temp;
		}
	}

	Rect(Vec<T> v0, Vec<T> v1)
	{
		X0 = v0.X;
		Y0 = v0.Y;
		X1 = v1.X;
		Y1 = v1.Y;

		if (X0 > X1)
		{
			T temp = X1;
			X1 = X0;
			X0 = temp;
		}

		if (Y0 > Y1)
		{
			T temp = Y1;
			Y1 = Y0;
			Y0 = temp;
		}
	}

	static Rect FromCent(Vec<T> center, T halfWidth, T halfHeight)
	{
		Vec<T> half = { halfWidth, halfHeight };
		return Rect(center - half, center + half);
	}

	bool CollWith(const Rect& targ) const
	{
		return ((X1 > targ.X0) && (X0 < targ.X1)
			&& (Y1 > targ.Y0) && (Y0 < targ.Y1));
	}

	bool CollWith(const Vec<T>& targ) const
	{
		return ((X1 > targ.X) && (X0 < targ.X)
			&& (Y1 > targ.Y) && (Y0 < targ.Y));
	}

	T width() const
	{
		return X1 - X0;
	}

	T height() const
	{
		return Y1 - Y0;
	}

	Rect GetExpand(T offset) const
	{
		return Rect<T>(X0 - offset, Y0 - offset, X1 + offset, Y1 + offset);
	}

	Rect GetExpand(T offx, T offy) const
	{
		return Rect<T>(X0 - offx, Y0 - offy, X1 + offx, Y1 + offy);
	}

	Rect GetExpand(Vec<T> offset) const
	{
		return GetExpand(offset.X, offset.Y);
	}

	Vec<T> Cent() const
	{
		return { (X1 + X0) / (T)2, (Y1 + Y0) / (T)2, };
	}

public:
	T X0 = T(0);
	T Y0 = T(0);
	T X1 = T(0);
	T Y1 = T(0);
};