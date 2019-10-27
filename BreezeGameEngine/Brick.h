#pragma once

#include "Graphics.h"
#include "RectF.h"
#include "Ball.h"
#include "Beveler.h"

class Brick
{
public:
	Brick() = default;
	Brick(RectF rect, Color c);
	void Draw(Graphics& gfx);
	bool CollCheck(const Ball ball) const;
	void Break(Ball& ball);
	Vec GetLoc() const;
	RectF Rect;
	void Respawn();

private:
	Beveler Bev;
	bool Broken = 0;
};