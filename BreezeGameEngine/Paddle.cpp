#include "Paddle.h"
#include "Keyboard.h"

Paddle::Paddle(Vec pos, float hWidth, float hHeight)
{
	Pos = pos;
	halfWidth = hWidth;
	halfHeight = hHeight;
}

void Paddle::Draw(Graphics& gfx) const
{
	RectF rect = GetRect();
	gfx.DrawRect(rect, wingC);
	rect.X0 += wingWidth;
	rect.X1 -= wingWidth;
	gfx.DrawRect( rect, C);
}

void Paddle::Move(const Keyboard& kbd, const float dt)
{
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		Pos.X -= Vx * dt * 60.0f;
	}
	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		Pos.X += Vx * dt * 60.0f;
	}
}

bool Paddle::checkColl(Ball& ball) const
{
	return GetRect().CollWith(ball.GetRect());
}

bool Paddle::CollBall(Ball& ball)
{
	if (!onCooldown && checkColl(ball))
	{
		onCooldown = 1;
		Vec bpos = ball.GetPos();
		Vec off = (bpos - Pos);

		if (signbit(ball.GetVel().X) == signbit(off.X))
		{
			bpos.X = 1.73205f * (bpos.X / halfWidth) * bpos.Y; // Up to 60 deg angle
			ball.BounceY(off);
		}

		else if (bpos.X > Pos.X - halfWidth && bpos.X < Pos.X+halfWidth)
		{
			bpos.X = bpos.X * bpos.Y / halfWidth;
			ball.BounceY(off);
		}

		else
		{
			ball.BounceX();
		}

		return 1;
	}
	return 0;
}

void Paddle::CollWall(RectF& wall)
{
	RectF Rect = GetRect();
	if (Rect.X0 < wall.X0)
	{
		Pos.X += wall.X0-Rect.X0;
	}
	if (Rect.X1 > wall.X1)
	{
		Pos.X += wall.X1 - Rect.X1;
	}
}

RectF Paddle::GetRect() const
{
	return RectF::FromCent(Pos, halfWidth, halfHeight);
}

void Paddle::Cool()
{
	onCooldown = 0;
}

float Paddle::height() const
{
	return halfHeight;
}
