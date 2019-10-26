#include "Brick.h"
#include <assert.h>

Brick::Brick(RectF rect, Color c)
{
	Rect = rect;
	C = c;
}

void Brick::Draw(Graphics& gfx)
{
	if (!Broken)
	{
		gfx.DrawRect(Rect.GetExpand(-1.0f), C);
	}
}

bool Brick::CollCheck(const Ball ball) const
{
	return (!Broken && Rect.CollWith(ball.GetRect()));
}

void Brick::Break(Ball& ball)
{
	assert(CollCheck(ball));

	Vec bpos = ball.GetPos();

	if (signbit(ball.GetVel().X) == signbit((bpos - GetLoc()).X) )
	{
		ball.BounceY();
	}

	else if (bpos.X > Rect.X0&& bpos.X < Rect.X1)
	{
		ball.BounceY();
	}

	else
	{
		ball.BounceX();
	}

	Broken = 1;
}

Vec Brick::GetLoc() const
{
	return Rect.Cent();
}

void Brick::Respawn()
{
	Broken = 0;
}
