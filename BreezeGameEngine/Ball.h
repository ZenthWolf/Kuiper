#pragma once
#include "Graphics.h"
#include "Vec.h"
#include "RectF.h"

class Ball 
{
public:
	Ball(Graphics& gfx);
	Ball(Graphics& gfx, float x, float y, Color c);
	Ball(Graphics& gfx, float x, float y, float vx, float vy, Color c);
	void Draw();
	void Update(float dt);

private:
	RectF Rect;
	Vec Vel;
	static constexpr int R = 17;
	Color C;
	Graphics& gfx;
};