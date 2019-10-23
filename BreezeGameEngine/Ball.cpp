#include "Ball.h"

Ball::Ball(Graphics& gfx) : gfx(gfx)
{
}

Ball::Ball(Graphics& gfx, float x, float y, Color c) : gfx(gfx)
{
	X = x;
	Y = y;
	C = c;
}

Ball::Ball(Graphics& gfx, float x, float y, float vx, float vy, Color c) : gfx(gfx)
{
	X = x;
	Y = y;
	Vx = vx;
	Vy = vy;
	C = c;
}

void Ball::Draw()
{
	gfx.DrawCirc(int(X), int(Y), R, C);
}
