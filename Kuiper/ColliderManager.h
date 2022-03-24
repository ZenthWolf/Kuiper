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
#include "Ship.h"
#include "Asteroid.h"
#include "Shapes.h"

struct ActiveEdge;

class ColliderManager
{
public:
	ColliderManager(std::vector<std::unique_ptr<Asteroid>>& belt, Ship& ship)
		:belt(belt), ship(ship)
	{}
	void DoCollisions(const float dt, bool collship, int& jank) const; //originally CollCheck

private:
	//Collision Depth Analyzers
	void DoBinaryCollision(Entity& BodyA, Entity& BodyB, const float dt, int& jank, bool doJank = false) const;
	float ResolveMidField(Entity& BodyA, Entity& BodyB, ActiveEdge& contactEdge) const; //originally CollCheck
	void ResolveNearField(const std::vector<Vec<float>>& InitBodyA0, const std::vector<Vec<float>>& InitBodyA1,
						const std::vector<Vec<float>>& InitBodyB0, const std::vector<Vec<float>>& InitBodyB1,
						float& t, ActiveEdge& outfoundEdge) const;
	void ResolveDiscreteCollision(const std::vector<Vec<float>>& BodyA, const std::vector<Vec<float>>& BodyB,
								float& t, ActiveEdge& outfoundEdge) const;
	void UpdateEdge(ActiveEdge& outEdge, const Approach& approach, 
						const std::vector<Vec<float>>& A0, const std::vector<Vec<float>>& A1, 
						const std::vector<Vec<float>>& B0, const std::vector<Vec<float>>& B1) const;
	ActiveEdge DeepestVsEdgeSolver(const Vec<float>& edgeI0, const Vec<float>& edgeJ0,
								const Vec<float>& edgeI1, const Vec<float>& edgeJ1,
								const std::vector<Vec<float>>& pointCloud) const;
	float FindRoot(const ActiveEdge& curEdge, const float& T0init, const float& T1init) const;
	std::vector<Vec<float>> BodyAtTime(const std::vector<Vec<float>>& Body0, 
									const std::vector<Vec<float>>& Body1, const float timeParam) const;


	const float tolerance = 0.01f;
	std::vector<std::unique_ptr<Asteroid>>& belt;
	Ship& ship;
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
	bool discreteCollision = false;

	// caches deepest point value
	float depth1 = -FLT_MAX;
};
