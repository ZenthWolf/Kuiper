#pragma once

#include "VecI.h"

class RectI 
{
public:
	RectI() = default;
	RectI(int x0, int y0, int x1, int y1);
	RectI(VecI v0, VecI v1);
	static RectI FromCent(VecI center, int halfWidth, int halfHeight);
	bool CollWith( const RectI& Targ ) const;
	int X0; int Y0; int X1; int Y1;
	int width() const;
	int height() const;
	RectI GetExpand(int offset) const;
	RectI GetExpand(int offx, int offy) const;
	VecI Cent() const;
};