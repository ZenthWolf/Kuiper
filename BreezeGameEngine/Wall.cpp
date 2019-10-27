#include "Wall.h"

Wall::Wall(RectF boundary, float thickness, Color c)
{
	Bound = boundary;
	borderThickness = thickness;
	C = c;
}

RectF Wall::GetBound()
{
	return Bound;
}

Vec Wall::GetOrigin()
{
	return { Bound.X0, Bound.Y0 };
}

void Wall::Draw(Graphics& gfx)
{
	RectF LeftB = RectF(Bound.X0 - borderThickness, Bound.Y0 - borderThickness, Bound.X0, Bound.Y1 + borderThickness );
	RectF RightB = RectF(Bound.X1, Bound.Y0 - borderThickness, Bound.X1 + borderThickness, Bound.Y1+borderThickness );
	RectF TopB = RectF(Bound.X0 - borderThickness, Bound.Y0 - borderThickness, Bound.X1 + borderThickness, Bound.Y0);
	RectF BotB = RectF(Bound.X0 - borderThickness, Bound.Y1, Bound.X1 + borderThickness, Bound.Y1 + borderThickness);

	gfx.DrawRect(LeftB, C);
	gfx.DrawRect(RightB, C);
	gfx.DrawRect(TopB, C);
	gfx.DrawRect(BotB, C);
}
