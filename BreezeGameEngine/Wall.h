#pragma once

#include "RectF.h"
#include "Graphics.h"
#include "Beveler.h"

class Wall 
{
public:
	Wall(RectF boundary, Color c);
	RectF GetBound();
	Vec GetOrigin();
	void Draw(Graphics& gfx);

private:
	RectF Bound;
	Beveler Bev;
	static constexpr float borderThickness = 3.0f;
};