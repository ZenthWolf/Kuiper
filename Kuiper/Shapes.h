#pragma once

#include<vector>
#include<list>
#include "Engine/Vec.h"

/* Shapes use counterclockwise unwinding. Important for defining normals */

class Shapes 
{
public:
	static std::vector<Vec<float>> Ship()
	{
		std::vector<Vec<float>> ship;
		ship.reserve(5);
		ship.emplace_back(20.0f, 0.0f);
		ship.emplace_back(-20.0f, 10.0f);
		ship.emplace_back(-25.0f, 5.0f);
		//ship.emplace_back(-15.0f, 5.0f);
		ship.emplace_back(-25.0f, -5.0f);
		//ship.emplace_back(-15.0f, -5.0f);
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

		for (int i = 0; i < 10; ++i)
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
		asteroid.emplace_back(-15.27272727f, -8.27272727f);
		asteroid.emplace_back(-3.27272727f, -10.27272727f);
		asteroid.emplace_back(8.72727272f, -8.27272727f);
		asteroid.emplace_back(13.72727272f, 2.72727272f);
		asteroid.emplace_back(16.72727272f, 8.72727272f);
		asteroid.emplace_back(10.72727272f, 14.72727272f);
		asteroid.emplace_back(5.72727272f, 12.72727272f);
		asteroid.emplace_back(-3.27272727f, 8.72727272f);
		asteroid.emplace_back(-7.27272727f, 11.72727272f);
		asteroid.emplace_back(-12.27272727f, 0.27272727f);
		asteroid.emplace_back(-14.27272727f, -5.27272727f);
		return asteroid;
	}

	static std::list<std::vector<Vec<float>>> ConvexSeparator(std::vector<Vec<float>>& model);

	static Vec<float> FindIntersection(const Vec<float>& p0, const Vec<float>& p1, const Vec<float>& r0, const Vec<float>& r1);
	static bool IsOnSegment(const Vec<float>& p, const Vec<float>& l0, const Vec<float>& l1);
	static bool IsSamePoint(const Vec<float>& p, const Vec<float>& r);
};