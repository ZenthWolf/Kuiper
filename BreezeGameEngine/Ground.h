#pragma once

#include "Graphics.h"
#include "Rect.h"

class Ground
{
public:
	Ground(float x0, float x1, float y, Color dirt = { 150, 75, 0 });
	Rect<float> Rect() const;
	void Draw(Graphics& gfx) const;
	float sayY() const;

private:
	float X0;
	float X1;
	float Y;

	Color dirtColor;
};