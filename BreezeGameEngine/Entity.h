#pragma once

#include <vector>
#include "Vec.h"

class Entity
{
public:
	Entity(std::vector<Vec<float>> model)
		:model(std::move(model))
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

	std::vector<Vec<float>> GetPolyline() const
	{
		auto poly = model;
		for (auto& v : poly)
		{
			v += pos;
		}
		return poly;
	}

private:
	std::vector<Vec<float>> model;

	Vec<float> pos = { 0.0f, 0.0f };
};