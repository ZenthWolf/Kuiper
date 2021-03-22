#include "CoordinateTransformer.h"

void CoordinateTransformer::Draw(Drawable& drawable) const
{
	const Vec<float> offset = { float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight / 2) };

	drawable.ScaleY(-1.0f);
	drawable.Translate(offset);
	drawable.Render(gfx);
}