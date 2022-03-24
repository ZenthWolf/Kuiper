/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#pragma once

#include<memory>
#include <random>
#include "Asteroid.h"

struct NearElements
{
	enum class Type { Vertex, Edge };

	int v0;
	Type type0;
	int v1;
	Type type1;

	bool flip;
};

class Spawner
{
public:
	Spawner(std::vector<std::unique_ptr<Asteroid>>& belt);

	void Update(const float dt, const Rect<float> cambox);

	int FindIncidentEdge(const int edge, 
		const std::vector<Vec<float>>& source, 
		const std::vector<Vec<float>>& target) const;
	float FindMaxSeparation(int& edge,
		const std::vector<Vec<float>>& source,
		const std::vector<Vec<float>>& target) const;
	NearElements GetNearestElements(const std::vector<Vec<float>>& model0,
		const std::vector<Vec<float>>& model1) const;

private:
	void GenerateAsteroid(const Rect<float> cambox);

	std::vector<std::unique_ptr<Asteroid>>& belt;
	float GenTime = 0.0f;
	std::mt19937 rng;

	float vRange = 300.0f;
	float sMin = 0.75f;
	float sMax = 5.0f;
	float omgRange = 5.0f;

	int forcedAst = 5;
};