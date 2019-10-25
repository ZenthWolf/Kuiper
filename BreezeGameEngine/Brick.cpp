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
		gfx.DrawRect(Rect, C);
	}
}

bool Brick::Break(Ball& ball)
{
	if (Rect.CollWith(ball.GetRect()) && !Broken )
	{
		ball.BounceY();
		Broken = 1;
		return 1;
	}
	return 0;
}
