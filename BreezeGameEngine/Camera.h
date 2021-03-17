#pragma once

#include "CoordinateTransformer.h"

class Camera
{
public:
	Camera( CoordinateTransformer& ct)
		:ct(ct)
	{}

	Vec<float> GetPos() const
	{
		return pos;
	}

	void Move(const Vec<float>& offset)
	{
		pos += offset;
	}

	void MoveTo(const Vec<float>& newpos)
	{
		pos = newpos;
	}

	void DrawPolylineC(std::vector<Vec<float>> poly, Color c)
	{
		for (auto& v : poly)
		{
			v -= pos;
		}

		ct.DrawPolylineC(std::move(poly), c);
	}

private:
	Vec<float> pos = {0.0f, 0.0f};
	CoordinateTransformer& ct;
};