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

#include "CoordinateTransformer.h"

CoordinateTransformer::CoordinateTransformer(Graphics& gfx)
	: gfx(gfx)
{}

void CoordinateTransformer::Draw(Drawable& drawable, bool debugJoints) const
{
	const Vec<float> offset = { float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight / 2) };

	drawable.ScaleY(-1.0f);
	drawable.Translate(offset);
	drawable.Render(gfx, debugJoints);
}