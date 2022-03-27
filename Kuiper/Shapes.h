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

#include<vector>

#include "Engine/Vec.h"
#include "Engine/Rect.h"

//Forward Declarations
struct Approach;
struct Simplex;
struct SimplexVertex;

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
		
		//ship.emplace_back(-25.0f, 5.0f);
		ship.emplace_back(-15.0f, 5.0f);
		
		//ship.emplace_back(-25.0f, -5.0f);
		ship.emplace_back(-15.0f, -5.0f);
		
		ship.emplace_back(-20.0f, -10.0f);

		Center(ship);

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

		Center(star);
		return star;
	}

	static std::vector<Vec<float>> Asteroid()
	{
		std::vector<Vec<float>> asteroid;
		asteroid.reserve(11);
		asteroid.emplace_back(-8.27272727f, 15.27272727f);
		asteroid.emplace_back(-10.27272727f, 3.27272727f);
		asteroid.emplace_back(-8.27272727f, -8.72727272f);
		asteroid.emplace_back(2.72727272f, -13.72727272f);
		asteroid.emplace_back(8.72727272f, -16.72727272f);
		asteroid.emplace_back(14.72727272f, -10.72727272f);
		asteroid.emplace_back(12.72727272f, -5.72727272f);
		asteroid.emplace_back(8.72727272f, 3.27272727f);
		asteroid.emplace_back(11.72727272f, 7.27272727f);
		asteroid.emplace_back(0.27272727f, 12.27272727f);
		asteroid.emplace_back(-5.27272727f, 14.27272727f);
		
		
		/*
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
		*/
		Center(asteroid);
		return asteroid;
	}

	static std::vector<Vec<float>> Hexabot()
	{
		std::vector<Vec<float>> hexabotSchematic;
		hexabotSchematic.reserve(14);
		hexabotSchematic.emplace_back(-.5f, 0.86602540378f);
		hexabotSchematic.emplace_back(-1.0f, 0.0f);
		hexabotSchematic.emplace_back(-.5f, -0.86602540378f);
		hexabotSchematic.emplace_back(.5f, -0.86602540378f);
		hexabotSchematic.emplace_back(1.0f, 0.0f);
		hexabotSchematic.emplace_back(.5f, 0.86602540378f); //Hexagon

		hexabotSchematic.emplace_back(.6f, 0.86602540378f + 0.25f); //OutterEdge of Right
		hexabotSchematic.emplace_back(.385f, 0.86602540378f + 0.5f); //Point of Right
		hexabotSchematic.emplace_back(.4f, 0.86602540378f + 0.2f); // Bend of Right
		hexabotSchematic.emplace_back(.25f, 0.86602540378f); //hex

		hexabotSchematic.emplace_back(-.25f, 0.86602540378f);  //hex
		hexabotSchematic.emplace_back(-.4f, 0.86602540378f + 0.2f); // Bend of Left
		hexabotSchematic.emplace_back(-.385f, 0.86602540378f + 0.5f); //Point of Left
		hexabotSchematic.emplace_back(-.6f, 0.86602540378f + 0.25f); //Outter Edge of Left

		//Center(hexabotSchematic); //Hex is centered, pincers are not to be included
		return hexabotSchematic;
	}

	static std::vector<std::vector<Vec<float>>> ConvexSeparator(const std::vector<Vec<float>>& model);
	static void Center(std::vector<Vec<float>>& shape);

	//Basic Geometric Functions
	static Vec<float> FindIntersection(const Vec<float>& p0, const Vec<float>& p1, const Vec<float>& r0, const Vec<float>& r1);
	static bool IsOnSegment(const Vec<float>& p, const Vec<float>& l0, const Vec<float>& l1);
	static bool IsSamePoint(const Vec<float>& p, const Vec<float>& r);

	//GJK Calculation
	static Approach FindApproach(const std::vector<std::vector<Vec<float>>>& modelList0,
						const std::vector<std::vector<Vec<float>>>& modelList1);
	static Approach FindApproach(const std::vector<Vec<float>>& model0, 
						const std::vector<Vec<float>>& model1);
	static int FindSupport(const Vec<float>& d, const std::vector<Vec<float>>& model);

	//Fallback Utilities
	static Rect<float> BoundingBox(const std::vector<Vec<float>>& convexPoly);
};

/*******************/
/* DATA STRUCTURES */
/*******************/
struct Approach
{
	enum class Type { Vertex, Edge };
	Vec<float> point0;      // closest point on polygon 0
	int convex0 = -1;       // convex primitive of point0
	int index0 = -1;        // index of the element index0 represents
	Type type0;             // the type of element index0 represents

	Vec<float> point1;      // closest point on polygon 1
	int convex1 = -1;       // convex primitive of point1
	int index1 = -1;        // index of the element index1 represents
	Type type1;             // the type of element index1 represents

	float distance;
	int iterations;		    // number of GJK iterations used
};

/// A simplex vertex for the Minkowski difference in GJK
struct SimplexVertex
{
	Vec<float> point0;	  // Point from polygon 0 in world space
	Vec<float> point1;	  // Point from polygon 1 in world space
	Vec<float> point;	  // Minkowski difference point

	float u;	          // unnormalized barycentric coordinate for closest point
	int index0 = -1;			  // point index in polygon 0
	int index1 = -1;			  // point index in polygon 1
};


/// Simplex used by the GJK algorithm.
struct Simplex
{
	Vec<float> GetSearchDirection() const;
	void FindNearestToOrigin();
	Approach PrepareResult(const int iter, const int convex0, const int convex1) const;

	SimplexVertex vertex0, vertex1, vertex2;
	float divisor; //Normalizes barycentric coords
	int count;
private:
	void Solve2();
	void Solve3();
};