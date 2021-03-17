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
	void Draw( Drawable& drawable ) const
	{
		const Vec<float> offset = { float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight / 2) };

		drawable.ScaleY(-1.0f);
		drawable.Translate(offset);
		drawable.Render(gfx);
	}

private:
	Graphics& gfx;
};