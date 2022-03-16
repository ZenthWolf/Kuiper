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
	enum class Type { Vertex, Edge };
	Vec<float> point0;		// closest point on polygon 0
	int convex0;            // convex primitive of point0
	int index0;             // index of the element index0 represents
	Type type0;             // the type of element index0 represents

	Vec<float> point1;		// closest point on polygon 1
	int convex1;            // convex primitive of point1
	int index1;             // index of the element index1 represents
	Type type1;             // the type of element index1 represents

	float distance;
	int iterations;		    // number of GJK iterations used
};

struct NearElements
{
	enum class Type { Vertex, Edge };

	int v0;
	Type type0;
	int v1;
	Type type1;

	bool flip;
};

/// Simplex used by the GJK algorithm.
struct Simplex
{
	Vec<float> GetSearchDirection() const;
	void Solve2();
	void Solve3();
	Approach PrepareResult(int iter, int convex0, int convex1);

	SimplexVertex vertex0, vertex1, vertex2;
	float divisor; //Normalizes barycentric coords
	int count;
};

struct ActiveEdge
{
	Vec<float> n0;
	Vec<float> n1;
	float planeConst0;
	float planeConst1;

	Vec<float> p0;
	Vec<float> p1;
	int pInd;
	bool edgeIsA;

	// caches deepest point value
	float depth1 = -FLT_MAX;
};

class Spawner
{
public:
	Spawner(std::vector<std::unique_ptr<Asteroid>>& belt);

	void Update(const float dt, const Rect<float> cambox);
	void CollideShip(Entity& ship, float dt);

	int FindIncidentEdge(const int edge, 
		const std::vector<Vec<float>>& source, 
		const std::vector<Vec<float>>& target) const;
	float FindMaxSeparation(int& edge,
		const std::vector<Vec<float>>& source,
		const std::vector<Vec<float>>& target) const;
	Approach FindApproach(const std::vector<Vec<float>>& model0, const std::vector<Vec<float>>& model1) const;
	Approach FindApproach(const std::vector<std::vector<Vec<float>>>& modelList0,
						  const std::vector<std::vector<Vec<float>>>& modelList1) const;
	NearElements GetNearestElements(const std::vector<Vec<float>>& model0,
		const std::vector<Vec<float>>& model1) const;

private:
	void CollCheck(float dt);
	float CollCheck(Entity& A, Entity& B, std::unique_ptr<ActiveEdge>& contactEdge);
	void ResolveNearField(std::vector<Vec<float>>& BodyA0, std::vector<Vec<float>>& BodyA1,
						std::vector<Vec<float>>& BodyB0, std::vector<Vec<float>>& BodyB1,
						float& t, std::unique_ptr<ActiveEdge>& foundEdge);
	ActiveEdge DeepestVsEdgeSolver(Vec<float> edgeI0, Vec<float> edgeJ0,
								Vec<float> edgeI1, Vec<float> edgeJ1,
								std::vector<Vec<float>> pointCloud
								);
	float FindRoot(ActiveEdge curEdge, float T0, float T1);
	std::vector<Vec<float>> BodyAtTime(std::vector<Vec<float>> Body0, std::vector<Vec<float>> Body1, float time);

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