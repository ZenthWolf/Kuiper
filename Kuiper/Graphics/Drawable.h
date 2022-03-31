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

#include<math.h>
#include<vector>

#include "Engine/Vec.h"
#include "Graphics/Colors.h"
#include "Graphics/Graphics.h"

class Drawable
{
public:
	Drawable(const std::vector<Vec<float>>& model, Color c);

	void Translate(const Vec<float> ds);
	void Scale(const float s);
	void Rot(const float th);

	void ScaleInd(const float sx, const float sy);
	void ScaleX(const float sx);
	void ScaleY(const float sy);

	void Render(Graphics& gfx, bool debugJoints = true) const;

private:
	const std::vector<Vec<float>>* model;
	Vec<float> translation = { 0.0f, 0.0f };
	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float rot = 0.0f;
	Color c;
};