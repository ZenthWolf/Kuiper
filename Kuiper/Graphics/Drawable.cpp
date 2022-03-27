/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

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

void Drawable::Render(Graphics& gfx, bool debugJoints) const
{
	gfx.DrawPolylineC(*model, translation, scale_x, scale_y, rot, c, debugJoints);
}