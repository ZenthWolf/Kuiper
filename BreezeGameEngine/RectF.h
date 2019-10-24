#pragma once
#include "Graphics.h"
#include "Vec.h"

class RectF 
{
public:
	RectF(Graphics& gfx);
	RectF(Graphics& gfx, float x, float y, float w, float h, Color c);
	Vec Pos;
	float W;
	float H;
	Color C;
	void Draw();
	bool CollWith( const RectF& Targ ) const;

private:
	Graphics& gfx;
};