#include "Ground.h"

Ground::Ground(float x0, float x1, float y, Color dirt)
	:X0(x0), X1(x1), Y(y), dirtColor(dirt)
{
}

Rect<float> Ground::Rect() const
{
	return { X0, Y, X1, 600.0f };
}

void Ground::Draw(Graphics& gfx) const
{
	gfx.DrawRect(Rect(), dirtColor);
}

float Ground::sayY() const
{
	return Y;
}
