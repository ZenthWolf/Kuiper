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
	gfx.DrawRectDim(int(X),int(Y),int(W),int(H),C);
}

bool Rect::CollWith(const Rect& Targ) const
{
	return ( (X+W > Targ.X) && (X < Targ.X + Targ.W) 
	      && (Y+H > Targ.Y) && (Y < Targ.Y + Targ.W));
}
