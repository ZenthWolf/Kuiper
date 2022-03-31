/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#pragma once

#include "Engine/Vec.h"
#include "Graphics/Graphics.h"
#include "Graphics/Drawable.h"

class CoordinateTransformer
{
public:
	CoordinateTransformer(Graphics& gfx);

	void Draw(Drawable& drawable, bool debugJoints = true) const;

private:
	Graphics& gfx;
};