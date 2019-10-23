#include "Brick.h"

Brick::Brick(Graphics& gfx) : gfx(gfx), Rect(gfx)
{
}

Brick::Brick(Graphics& gfx, int x, int y, Color c): gfx(gfx), Rect(gfx, float(x), float(y), W, H, c)
{
	X = x;
	Y = y;
	C = c;
}

void Brick::Draw() const
{
	gfx.DrawRectDim(X, Y, W, H, C);
}
