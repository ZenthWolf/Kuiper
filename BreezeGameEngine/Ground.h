#pragma once

#include "Rect.h"
#include "Colors.h"
#include "Graphics.h"

class Ground
{
public:
	Ground(const Vec<float> v0, const Vec<float> v1);
	void Draw(Graphics& gfx) const;
	Rect<float> GetRect();

private:
	Rect<float> box;
	Color col = Colors::Gray;
};
