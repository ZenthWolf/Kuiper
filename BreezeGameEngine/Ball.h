#pragma once
#include "Graphics.h"
#include "Vec.h"
#include "RectF.h"
#include "Paddle.h"

class Ball 
{
public:
	Ball(Graphics& gfx);
	Ball(Graphics& gfx, float x, float y, Color c);
	Ball(Graphics& gfx, float x, float y, float vx, float vy, Color c);
	void Draw();
	void Update(float dt, const RectF& pad);
	bool Paddled(const RectF& pad) const;

private:
	RectF Rect;
	Vec Vel;
	static constexpr int R = 10;
	Color C;
	Graphics& gfx;
};