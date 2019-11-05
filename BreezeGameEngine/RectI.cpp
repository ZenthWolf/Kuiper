#include "RectI.h"
#include <math.h>

RectI::RectI(int x0, int y0, int x1, int y1)
{
	X0 = x0;
	Y0 = y0;
	X1 = x1;
	Y1 = y1;
}

RectI::RectI(VecI v0, VecI v1)
{
	X0 = v0.X;
	Y0 = v0.Y;
	X1 = v1.X;
	Y1 = v1.Y;
}

RectI RectI::FromCent(VecI center, int halfWidth, int halfHeight)
{
	VecI half = { halfWidth, halfHeight };
	return RectI(center-half, center+half);
}

bool RectI::CollWith(const RectI& Targ) const
{
	return ( (X1 >= Targ.X0) && (X0 <= Targ.X1)
	      && (Y1 >= Targ.Y0) && (Y0 <= Targ.Y1) );
}

int RectI::width() const
{
	return X1 - X0;
}

int RectI::height() const
{
	return Y1 - Y0;
}

RectI RectI::GetExpand(int offset) const
{
	return RectI(X0-offset, Y0-offset, X1+offset, Y1+offset);
}

RectI RectI::GetExpand(int offx, int offy) const
{
	return RectI(X0 - offx, Y0 - offy, X1 + offx, Y1 + offy);
}

VecI RectI::Cent() const
{
	return { (X1 + X0) / 2, (Y1 + Y0) / 2, };
}
