#pragma once

#include<vector>
#include<math.h>
#include "Vec.h"
#include "Graphics.h"

class Drawable
{
public:
	Drawable(std::vector<Vec<float>> model, Color c)
		:model(std::move(model)), c(c)
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
		for (auto& v : model)
		{
			float vx = v.X;
			float vy = v.Y;

			v.X = vx*cos(th) - vy*sin(th);
			v.Y = vx * sin(th) + vy * cos(th);
		}
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
		for (auto& v : model)
		{
			v.X *= scale_x;
			v.Y *= scale_y;
			v += translation;
		}
		gfx.DrawPolylineC(model, c);
	}

private:
	std::vector<Vec<float>> model;
	Vec<float> translation = { 0.0f, 0.0f };
	float scale_x = 1.0f;
	float scale_y = 1.0f;
	Color c;
};