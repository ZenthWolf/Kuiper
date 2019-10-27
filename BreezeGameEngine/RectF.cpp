#include "RectF.h"
#include <math.h>

RectF::RectF(float x0, float y0, float x1, float y1)
{
	X0 = x0;
	Y0 = y0;
	X1 = x1;
	Y1 = y1;
}

RectF::RectF(Vec v0, Vec v1)
{
	X0 = v0.X;
	Y0 = v0.Y;
	X1 = v1.X;
	Y1 = v1.Y;
}

RectF RectF::FromCent(Vec center, float halfWidth, float halfHeight)
{
	Vec half = { halfWidth, halfHeight };
	return RectF(center-half, center+half);
}

bool RectF::CollWith(const RectF& Targ) const
{
	return ( (X1 >= Targ.X0) && (X0 <= Targ.X1)
	      && (Y1 >= Targ.Y0) && (Y0 <= Targ.Y1) );
}

float RectF::width() const
{
	return fabsf(X1 - X0);
}

float RectF::height() const
{
	return fabsf(Y1 - Y0);
}

RectF RectF::GetExpand(float offset) const
{
	return RectF(X0-offset, Y0-offset, X1+offset, Y1+offset);
}

RectF RectF::GetExpand(float offx, float offy) const
{
	return RectF(X0 - offx, Y0 - offy, X1 + offx, Y1 + offy);
}

Vec RectF::Cent() const
{
	return { (X1 + X0) / 2.0f, (Y1 + Y0) / 2.0f, };
}
