#pragma once

#include<vector>
#include<math.h>
#include "Vec.h"
#include "Graphics.h"

class Drawable
{
public:
	Drawable(const std::vector<Vec<float>>& model, Color c)
		:model(&model), c(c)
	{}

	void Translate(const Vec<float> ds)
	{
		translation += ds;
	}

	void Scale(float s)
	{
		scale_x *= s;
		scale_y *= s;
		translation *= s;
	}

	void Rot(float th)
	{
		rot = th;
	}

	void ScaleInd(float sx, float sy)
	{
		scale_x *= sx;
		scale_y *= sy;
		translation.X *= sx;
		translation.Y *= sy;
	}

	void ScaleX(float sx)
	{
		ScaleInd(sx, 1.0f);
	}

	void ScaleY(float sy)
	{
		ScaleInd(1.0f, sy);
	}

	void Render(Graphics& gfx)
	{
		gfx.DrawPolylineC(*model, translation, scale_x, scale_y, rot, c);
	}

private:
	const std::vector<Vec<float>>* model;
	Vec<float> translation = { 0.0f, 0.0f };
	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float rot = 0.0f;
	Color c;
};