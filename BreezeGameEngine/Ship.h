#pragma once

#include<vector>
#include "Vec.h"

class Ship 
{
public:
	static std::vector<Vec<float>> Make()
	{
		std::vector<Vec<float>> ship;
		ship.reserve(5);
		ship.emplace_back(0.0f, 20.0f);
		ship.emplace_back(-10.0f, -20.0f);
		ship.emplace_back(-5.0f, -15.0f);
		ship.emplace_back(5.0f, -15.0f);
		ship.emplace_back(10.0f, -20.0f);

		return ship;
	}
};