#pragma once

#include<vector>
#include<math.h>
#include "Vec.h"
#include "Graphics.h"
#include "Colors.h"

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

	void Render(Graphics& gfx) const;

private:
	const std::vector<Vec<float>>* model;
	Vec<float> translation = { 0.0f, 0.0f };
	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float rot = 0.0f;
	Color c;
};