#pragma once

#include <vector>
#include "Vec.h"

class Entity
{
public:
	Entity(std::vector<Vec<float>> model, const Vec<float>& pos = {0.0f, 0.0f}, Color c = Colors::White)
		:model(std::move(model)), pos(pos), c(c)
	{}

	const Vec<float>& GetPos() const
	{
		return pos;
	}

	void SetPos(const Vec<float> newpos)
	{
		pos = newpos;
	}

	void TranslateBy(const Vec<float> dpos)
	{
		pos += dpos;
	}

	void SetScale(const float s)
	{
		scale = s;
	}

	float GetScale() const
	{
		return scale;
	}

	void SetHeading(const float th)
	{
		heading = th;
		while (heading > (2 * 3.1415926))
		{
			heading -= (2 * 3.1415926);
		}
	}

	void RotBy(const float th)
	{
		heading += th;
		while (heading > (2 * 3.1415926))
		{
			heading -= (2 * 3.1415926);
		}
	}

	Vec<float> GetHeading() const
	{
		return {cos(heading), sin(heading)};
	}

	Drawable GetDrawable() const
	{
		Drawable d(model, c);
		d.Rot(heading);
		d.Scale(scale);
		d.Translate(pos);
		return d;
	}

	void SetColor(Color cnew)
	{
		c = cnew;
	}

protected:
	void SetModel(std::vector<Vec<float>> modelnew)
	{
		model = std::move(modelnew);
	}

private:
	std::vector<Vec<float>> model;

	Color c;
	Vec<float> pos = { 0.0f, 0.0f };
	float scale = 1.0f;
	float heading = 0.0f;
};