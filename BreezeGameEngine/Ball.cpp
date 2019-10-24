#include "Ball.h"

Ball::Ball(Graphics& gfx) : gfx(gfx), Rect(gfx)
{
}

Ball::Ball(Graphics& gfx, float x, float y, Color c) : gfx(gfx), Rect(gfx,x-R,y-R, float(2*R+1), float(2*R+1), c)
{
	C = c;
}

Ball::Ball(Graphics& gfx, float x, float y, float vx, float vy, Color c) : gfx(gfx), Rect(gfx, x-R, y-R, float(2*R+1), float(2*R+1), c), Vel(vx, vy)
{
	C = c;
}

void Ball::Draw()
{
	gfx.DrawCirc(R+int(Rect.Pos.X), R+int(Rect.Pos.Y), R, C);
}

void Ball::Update(float dt, const RectF& pad)
{
	Rect.Pos.X += Vel.X * dt * 60.0f;
	Rect.Pos.Y += Vel.Y * dt * 60.0f;

	/*** Test Rects for Wall Behavior ***/
	float t = 5.0f;
	float SH = float(gfx.ScreenHeight);
	float SW = float(gfx.ScreenWidth);
	RectF Left(gfx, 0.0f, 0.0f, t, SH - 1.0f, Colors::Red);
	RectF Right(gfx, SW - t - 1.0f, 0.0f, t, SH - 1.0f, Colors::Red);
	RectF Top(gfx, 0.0f, 0.0f, SW - 1.0f, t, Colors::Red);
	RectF Bot(gfx, 0.0f, SH - t - 1.0f, SW - 1.0f, t, Colors::Red);

	if (Rect.CollWith(Left))
	{
		Rect.Pos.X = 5.0f;
		Vel.X = -Vel.X;
	}
	else if (Rect.CollWith(Right))
	{
		Rect.Pos.X = float(gfx.ScreenWidth) - 6.0f - 2*R;
		Vel.X = -Vel.X;
	}
	if (Rect.CollWith(Top))
	{
		Rect.Pos.Y = 5.0f;
		Vel.Y = -Vel.Y;
	}
	else if (Rect.CollWith(Bot))
	{
		Rect.Pos.Y = float(gfx.ScreenHeight) - 6.0f - 2*R;

		Vel.Y = -Vel.Y;
	}

	if (Paddled(pad))
	{
		Vel.Y = -Vel.Y;
	}
}

bool Ball::Paddled(const RectF& pad) const
{
	return Rect.CollWith(pad);
}
