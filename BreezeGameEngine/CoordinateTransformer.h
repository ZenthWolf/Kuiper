#pragma once

#include "Vec.h"
#include "Graphics.h"

class CoordinateTransformer
{
public:
	CoordinateTransformer(Graphics& gfx)
		: gfx(gfx)
	{}
	void DrawPolylineC(std::vector<Vec<float>> poly, Color c)
	{
		Vec<float> offset = { float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight / 2) };

		for (auto& v : poly)
		{
			v.Y *= -1.0f;
			v += offset;
		}

		gfx.DrawPolylineC(poly, c);
	}

private:
	Graphics& gfx;
};