#include "Ball.h"

Ball::Ball(Graphics& gfx) : gfx(gfx)
{
}

Ball::Ball(Graphics& gfx, float x, float y, Color c) : gfx(gfx), Pos(x,y)
{
	C = c;
}

Ball::Ball(Graphics& gfx, float x, float y, float vx, float vy, Color c) : gfx(gfx), Pos(x,y), Vel(vx, vy)
{
	C = c;
}

void Ball::Draw()
{
	gfx.DrawCirc(int(Pos.X), int(Pos.Y), R, C);
}
