#include "Brick.h"

Brick::Brick(RectF rect, Color c)
{
	Rect = rect;
	C = c;
}

void Brick::Draw(Graphics& gfx)
{
	gfx.DrawRect(Rect, C);
}

/*
void Brick::Broked(RectF ball)
{
	Broken = Rect.CollWith(ball);
}
*/