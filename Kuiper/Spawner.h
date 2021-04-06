#pragma once

#include<memory>
#include <random>
#include "Asteroid.h"

/// A simplex vertex.
struct SimplexVertex
{
	Vec<float> point0;	  // Point from polygon 0 in world space
	Vec<float> point1;	  // Point from polygon 1 in world space
	Vec<float> point;	  // Minkowski difference point

	float u;	          // unnormalized barycentric coordinate for closest point
	int index0;			  // point index in polygon 0
	int index1;			  // point index in polygon 1
};

struct Approach
{
	Vec<float> point0;		///< closest point on polygon 1
	Vec<float> point1;		///< closest point on polygon 2
	float distance;
	int iterations;		///< number of GJK iterations used
};

/// Simplex used by the GJK algorithm.
struct Simplex
{
	Vec<float> GetSearchDirection() const;
	void Solve2();
	void Solve3();
	Approach PrepareResult(int iter);

	SimplexVertex vertex0, vertex1, vertex2;
	float divisor; //Normalizes barycentric coords
	int count;
};

class Spawner
{
public:
	Spawner(std::vector<std::unique_ptr<Asteroid>>& belt);

	void Update(const float dt, const Rect<float> cambox);
	void CollideShip(Entity& ship);

	Approach FindApproach(const std::vector<Vec<float>>& model0, const std::vector<Vec<float>>& model1) const;

private:
	void CollCheck();
	void GenerateAsteroid(const Rect<float> cambox);

	int FindSupport(const Vec<float>& d, const std::vector<Vec<float>>& model) const;

	std::vector<std::unique_ptr<Asteroid>>& belt;
	float GenTime = 0.0f;
	std::mt19937 rng;

	float vRange = 300.0f;
	float sMin = 0.75f;
	float sMax = 5.0f;
	float omgRange = 5.0f;

	int forcedAst = 5;
};