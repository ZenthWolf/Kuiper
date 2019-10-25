#include "Ball.h"

Ball::Ball()
{
}

Ball::Ball(float x, float y, Color c)
{
	Pos = { x, y };
	C = c;
}

Ball::Ball(float x, float y, float vx, float vy, Color c)
{
	Pos = { x, y };
	Vel = { vx, vy };
	C = c;
}

void Ball::Draw(Graphics& gfx)
{
	gfx.DrawCirc(int(Pos.X), int(Pos.Y), int(R), C);
}

void Ball::Move(float dt)
{
	Pos += Vel * dt * 60.0f;
}

bool Ball::CollWall(const RectF& wall)
{
	RectF Rect = GetRect();

	bool hit = false;
	if (Rect.X0 < wall.X0)
	{
		Pos.X += wall.X0 - Rect.X0;
		BounceX();
		hit = true;
	}
	else if (Rect.X1 > wall.X1)
	{
		Pos.X += wall.X1 - Rect.X1;
		BounceX();
		hit = true;
	}
	if (Rect.Y0 < wall.Y0)
	{
		Pos.Y += wall.Y0 - Rect.Y0;
		BounceY();
		hit = true;
	}
	else if (Rect.Y1 > wall.Y1)
	{
		Pos.Y += wall.Y1 - Rect.Y1;
		BounceY();
		hit = true;
	}
	return hit;
}

void Ball::BounceX()
{
	Vel.X = -Vel.X;
}

void Ball::BounceY()
{
	Vel.Y = -Vel.Y;
}

void Ball::Bounce(const Vec impulse)
{
	Vel.Y = Vel.Y * impulse.Y;
	Vel.X = Vel.X * impulse.X;
}

Vec Ball::GetPos() const
{
	return Pos;
}

Vec Ball::GetVel() const
{
	return Vel;
}

RectF Ball::GetRect() const
{
	return RectF::FromCent(Pos, R, R);;
}
