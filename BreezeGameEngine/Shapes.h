#pragma once

#include<vector>
#include "Vec.h"

class Shapes 
{
public:
	static std::vector<Vec<float>> Ship()
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

	static std::vector<Vec<float>> ShipBurn()
	{
		std::vector<Vec<float>> shipburn;
		shipburn.reserve(10);
		shipburn.emplace_back(20.0f, 0.0f);
		shipburn.emplace_back(-20.0f, 10.0f);
		shipburn.emplace_back(-15.0f, 5.0f);
		shipburn.emplace_back(-19.0f, 3.0f);
		shipburn.emplace_back(-17.0f, 1.0f);
		shipburn.emplace_back(-18.0f, 0.0f);
		shipburn.emplace_back(-17.0f, -1.0f);
		shipburn.emplace_back(-19.0f, -3.0f);
		shipburn.emplace_back(-15.0f, -5.0f);
		shipburn.emplace_back(-20.0f, -10.0f);

		return shipburn;
	}

	static std::vector<Vec<float>> Star()
	{
		std::vector<Vec<float>> star;
		star.reserve(10);

		const float dtheta = 2.0f * 3.1415926f / 10.0f;

		for (int i = 0; i < 10; i++)
		{
			star.emplace_back(
				20.0f / (float(i % 2 + 1)) * cos(float(i+5) * dtheta ),
				20.0f / (float(i % 2 + 1)) * sin(float(i+5) * dtheta ) );
		}

		return star;
	}

	static std::vector<Vec<float>> Asteroid()
	{
		std::vector<Vec<float>> asteroid;
		asteroid.reserve(11);
		asteroid.emplace_back(-16.72727272f, -11.27272727f);
		asteroid.emplace_back(-4.72727272f, -13.27272727f);
		asteroid.emplace_back(8.72727272f, -11.27272727f);
		asteroid.emplace_back(13.72727272f, 0.27272727f);
		asteroid.emplace_back(16.72727272f, 6.27272727f);
		asteroid.emplace_back(10.72727272f, 12.27272727f);
		asteroid.emplace_back(5.72727272f, 10.27272727f);
		asteroid.emplace_back(-2.72727272f, 6.27272727f);
		asteroid.emplace_back(-6.72727272f, 9.27272727f);
		asteroid.emplace_back(-11.72727272f, -3.27272727f);
		asteroid.emplace_back(-13.72727272f, -8.27272727f);

		return asteroid;
	}

};