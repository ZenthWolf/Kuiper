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
		ship.emplace_back(20.0f, 0.0f);
		ship.emplace_back(-20.0f, 10.0f);
		ship.emplace_back(-15.0f, 5.0f);
		ship.emplace_back(-15.0f, -5.0f);
		ship.emplace_back(-20.0f, -10.0f);

		return ship;
	}
};