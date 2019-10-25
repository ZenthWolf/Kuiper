#include "Brick.h"

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

bool Brick::Break(Ball& ball)
{
	Vec bpos = ball.GetPos();

	if (!Broken && Rect.CollWith(ball.GetRect()) )
	{
		if (bpos.X > Rect.X0&& bpos.X < Rect.X1)
		{
			ball.BounceY();
		}
		else
		{
			ball.BounceX();
		}

		Broken = 1;
		return 1;
	}
	return 0;
}

Vec Brick::GetLoc() const
{
	return { (Rect.X1 + Rect.X0) / 2.0f, (Rect.Y1 + Rect.Y0) / 2.0f, };
}
