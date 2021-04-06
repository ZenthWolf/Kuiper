#pragma once

#include "../Engine/Vec.h"
#include "Graphics.h"
#include "Drawable.h"

class CoordinateTransformer
{
public:
	CoordinateTransformer(Graphics& gfx);

	void Draw(Drawable& drawable) const;

private:
	Graphics& gfx;
};