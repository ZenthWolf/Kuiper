#include "Drawable.h"

Drawable::Drawable(const std::vector<Vec<float>>& model, Color c)
	:model(&model), c(c)
{}

void Drawable::Translate(const Vec<float> ds)
{
	translation += ds;
}

void Drawable::Scale(const float s)
{
	scale_x *= s;
	scale_y *= s;
	translation *= s;
}

void Drawable::Rot(const float th)
{
	rot = th;
}

void Drawable::ScaleInd(const float sx, const float sy)
{
	scale_x *= sx;
	scale_y *= sy;
	translation.X *= sx;
	translation.Y *= sy;
}

void Drawable::ScaleX(const float sx)
{
	ScaleInd(sx, 1.0f);
}

void Drawable::ScaleY(const float sy)
{
	ScaleInd(1.0f, sy);
}

void Drawable::Render(Graphics& gfx) const
{
	gfx.DrawPolylineC(*model, translation, scale_x, scale_y, rot, c);
}