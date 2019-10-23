#include "Rect.h"

Rect::Rect(Graphics& gfx): gfx(gfx)
{
}

Rect::Rect(Graphics& gfx, float x, float y, float w, float h, Color c): gfx(gfx)
{
	X = x;
	Y = y;
	W = w;
	H = h;
	C = c;
}

void Rect::Draw()
{
	gfx.DrawRectDim(X,Y,W,H,C);
}
