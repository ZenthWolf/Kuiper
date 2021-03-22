#pragma once

#include "Vec.h"
#include "Graphics.h"
#include "Drawable.h"

class CoordinateTransformer
{
public:
	CoordinateTransformer(Graphics& gfx)
		: gfx(gfx)
	{}

	void Draw(Drawable& drawable) const;

private:
	Graphics& gfx;
};