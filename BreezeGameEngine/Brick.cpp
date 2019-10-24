#include "Brick.h"

Brick::Brick(Graphics& gfx) : gfx(gfx), Rect(gfx)
{
}

Brick::Brick(Graphics& gfx, float x, float y, Color c): gfx(gfx), Rect(gfx, x, y, W, H, c)
{
}

void Brick::Draw()
{
	Rect.Draw();
}
