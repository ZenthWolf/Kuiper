#include "RectF.h"

RectF::RectF(Graphics& gfx): gfx(gfx)
{
}

RectF::RectF(Graphics& gfx, float x, float y, float w, float h, Color c): gfx(gfx), Pos(x,y)
{
	W = w;
	H = h;
	C = c;
}

void RectF::Draw()
{
	gfx.DrawRectDim(int(Pos.X),int(Pos.Y),int(W),int(H),C);
}

bool RectF::CollWith(const RectF& Targ) const
{
	return ( (Pos.X+W >= Targ.Pos.X) && (Pos.X <= Targ.Pos.X + Targ.W)
	      && (Pos.Y+H >= Targ.Pos.Y) && (Pos.Y <= Targ.Pos.Y + Targ.H));
}
