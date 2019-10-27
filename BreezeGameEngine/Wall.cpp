#include "Wall.h"

Wall::Wall(RectF boundary, Color c):Bev(c)
{
	Bound = boundary;
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
	Bev.DrawBevBorder(GetBound(), borderThickness, gfx);
}
