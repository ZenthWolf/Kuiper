#pragma once

#include<memory>
#include <random>
#include "Asteroid.h"

/// A simplex vertex.
struct SimplexVertex
{
	Vec<float> point;	  // ?
	float u;	          // unnormalized barycentric coordinate for closest point
	int index;			  // point index
};

/// Simplex used by the GJK algorithm.
struct Simplex
{
	Vec<float> GetSearchDirection(const Vec<float>& Q) const;
	Vec<float> GetClosestPoint() const;
	void Solve2(const Vec<float>& Q);
	void Solve3(const Vec<float>& Q);

	SimplexVertex vertex0, vertex1, vertex2;
	float divisor; //Normalizes barycentric coords
	int count;
};

struct Approach
{
	Vec<float> point1;		///< closest point on polygon 1
	Vec<float> point2;		///< closest point on polygon 2
	float distance;
	int iterations;		///< number of GJK iterations used
};

class Spawner
{
public:
	Spawner(std::vector<std::unique_ptr<Asteroid>>& belt);

	void Update(const float dt, const Rect<float> cambox);
	void CollideShip(Entity& ship);

	Approach FindApproach(Vec<float> pnt, std::vector<Vec<float>> model) const;

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