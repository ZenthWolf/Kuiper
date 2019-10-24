#pragma once

#include "Graphics.h"
#include "Rect.h"

class Brick
{
public:
	Brick(Graphics& gfx);
	Brick(Graphics& gfx, float x, float y, Color c);
	void Draw();

private:
	static constexpr int W = 20;
	static constexpr int H = 5;
	Rect Rect;
	Color C;
	bool Broken = 0;
	Graphics& gfx;
};