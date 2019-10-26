#pragma once

#include "RectF.h"
#include "Graphics.h"

class Wall 
{
public:
	Wall(RectF boundary, float thickness, Color c);
	RectF GetBound();
	void Draw(Graphics& gfx);

private:
	RectF Bound;
	float borderThickness;
	Color C;
};