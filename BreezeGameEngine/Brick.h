#pragma once

#include "Graphics.h"
#include "RectF.h"

class Brick
{
	Brick(RectF rect, Color c);
	void Draw(Graphics& gfx);

private:
	RectF Rect;
	Color C;
	bool Broken = 0;
};