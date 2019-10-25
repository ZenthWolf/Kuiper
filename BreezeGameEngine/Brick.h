#pragma once

#include "Graphics.h"
#include "RectF.h"
#include "Ball.h"

class Brick
{
public:
	Brick() = default;
	Brick(RectF rect, Color c);
	void Draw(Graphics& gfx);
	bool Break(Ball& ball);
	Vec GetLoc() const;
	RectF Rect;
	Color C;

private:
	
	bool Broken = 0;
};