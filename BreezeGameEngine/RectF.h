#pragma once

#include "Vec.h"

class RectF 
{
public:
	RectF() = default;
	RectF(float x0, float y0, float x1, float y1);
	RectF(Vec v0, Vec v1);
	static RectF FromCent(Vec center, float halfWidth, float halfHeight);
	bool CollWith( const RectF& Targ ) const;
	float X0; float Y0; float X1; float Y1;
	float width() const;
	float height() const;
	RectF GetExpand(float offset) const;
	RectF GetExpand(float offx, float offy) const;
	Vec Cent() const;
};