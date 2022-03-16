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
#include<algorithm>
#include<cassert>
#include<iterator>
#define NOMINMAX

#include "Spawner.h"

/*Simplex Defs*/

/*
In Polygon-Polygon collisions, this "Simplex" is that of
the Minkowski Difference generated polygon
*/

//Search is relative, so normalizing not important
Vec<float> Simplex::GetSearchDirection() const
{
	switch (count)
	{
	case 1:
		return -vertex0.point;

	case 2:
	{
		Vec<float> edge = vertex1.point - vertex0.point;
		float sgn = edge.Cross(-vertex0.point);
		if (sgn > 0.0f)
		{
			// Origin is left of edge.
			return Vec<float>{-edge.Y, edge.X};
		}
		else
		{
			// Origin is right of edge.
			return Vec<float>{edge.Y, -edge.X};
		}
	}

	default:
		assert(false);
		return Vec<float>(0.0f, 0.0f);
	}
}

//Find closest point on this 1-Plex to a pnt
void Simplex::Solve2()
{
	Vec<float> A = vertex0.point;
	Vec<float> B = vertex1.point;

	//Sign flips to ensure positive results
	float u = -B.Dot(A - B);
	float v = -A.Dot(B - A);

	// Region A
	if (v <= 0.0f)
	{
		// New Simplex is vertex A.
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region B
	if (u <= 0.0f)
	{
		// New Simplex is vertex B, which needs to be moved.
		vertex0 = vertex1;
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region AB. 
	vertex0.u = u;
	vertex1.u = v;
	Vec<float> e = B - A;
	//if Degenerate, previous region was called.
	divisor = e.GetLengthSq();
	count = 2;
}

//Find closest point on this 2-Plex to a pnt
void Simplex::Solve3()
{
	Vec<float> A = vertex0.point;
	Vec<float> B = vertex1.point;
	Vec<float> C = vertex2.point;

	// Unnormalized edge barycentric coordinate
	float uAB = -B.Dot(A - B);
	float vAB = -A.Dot(B - A);

	float uBC = -C.Dot(B - C);
	float vBC = -B.Dot(C - B);

	float uCA = -A.Dot(C - A);
	float vCA = -C.Dot(A - C);

	//Check to for reduction to 0-Plex
	// Region A
	if (vAB <= 0.0f && uCA <= 0.0f)
	{
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region B
	if (uAB <= 0.0f && vBC <= 0.0f)
	{
		vertex0 = vertex1;
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region C
	if (uBC <= 0.0f && vCA <= 0.0f)
	{
		vertex0 = vertex2;
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Signed triangle area.
	float area = (B - A).Cross(C - A);

	// Unnormalized triangular barycentric coordinates
	// With above, tests that pnt is exterior a particular side
	float uQBC = B.Cross(C);
	float vAQC = C.Cross(A);
	float wABQ = A.Cross(B);

	// Region AB
	if (uAB > 0.0f && vAB > 0.0f && wABQ * area <= 0.0f)
	{
		vertex0.u = uAB;
		vertex1.u = vAB;
		Vec<float> e = B - A;
		divisor = e.Dot(e);
		count = 2;
		return;
	}

	// Region BC
	if (uBC > 0.0f && vBC > 0.0f && uQBC * area <= 0.0f)
	{
		vertex0 = vertex1;
		vertex1 = vertex2;

		vertex0.u = uBC;
		vertex1.u = vBC;
		Vec<float> e = C - B;
		divisor = e.GetLengthSq();
		count = 2;
		return;
	}

	// Region CA
	if (uCA > 0.0f && vCA > 0.0f && vAQC * area <= 0.0f)
	{
		//This is weird, investigate what swapping placement changes
		vertex1 = vertex0;
		vertex0 = vertex2;

		vertex0.u = uCA;
		vertex1.u = vCA;
		Vec<float> e = A - C;
		divisor = e.GetLengthSq();
		count = 2;
		return;
	}

	// Region ABC
	assert(abs(area) > 0.0f && uQBC * area > 0.0f && vAQC * area > 0.0f && wABQ * area > 0.0f);
	vertex0.u = uQBC;
	vertex1.u = vAQC;
	vertex2.u = wABQ;
	//Divisior non-zero, else previous region catches
	divisor = area;
	count = 3;
}

Approach Simplex::PrepareResult(int iter, int convex0, int convex1)
{
	float s = 1.0f / divisor;
	Approach result;

	result.convex0 = convex0;
	result.convex1 = convex1;

	switch (count)
	{
	case 1:
		result.point0 = vertex0.point0;
		result.index0 = vertex0.index0;
		result.type0 = Approach::Type::Vertex;

		result.point1 = vertex0.point1;
		result.index1 = vertex0.index1;
		result.type1 = Approach::Type::Vertex;
		break;

	case 2:
	{
		result.point0 = vertex0.point0 * (s * vertex0.u) + vertex1.point0 * (s * vertex1.u);

		if (abs(result.point0.X - vertex0.point0.X) < 0.001 && abs(result.point0.Y - vertex0.point0.Y) < 0.001)
		{
			result.index0 = vertex0.index0;
			result.type0 = Approach::Type::Vertex;
		}
		else if (abs(result.point0.X - vertex1.point0.X) < 0.001 && abs(result.point0.Y - vertex1.point0.Y) < 0.001)
		{
			result.index0 = vertex1.index0;
			result.type0 = Approach::Type::Vertex;
		}
		else
		{
			if ( (vertex0.index0 < vertex1.index0) && (vertex0.index0 + 1 == vertex1.index0) )
			{
				result.index0 = vertex0.index0;
			}
			else if ((vertex1.index0 < vertex0.index0) && (vertex1.index0 + 1 == vertex0.index0))
			{
				result.index0 = vertex1.index0;
			}
			else
			{
				result.index0 = std::max(vertex0.index0, vertex1.index0);
			}
			result.type0 = Approach::Type::Edge;
		}
		

		result.point1 = vertex0.point1 * (s * vertex0.u) + vertex1.point1 * (s * vertex1.u);
		if (abs(result.point1.X - vertex0.point1.X) < 0.001 && abs(result.point1.Y - vertex0.point1.Y) < 0.001)
		{
			result.index1 = vertex0.index1;
			result.type1 = Approach::Type::Vertex;
		}
		else if (abs(result.point1.X - vertex1.point1.X) < 0.001 && abs(result.point1.Y - vertex1.point1.Y) < 0.001)
		{
			result.index1 = vertex1.index1;
			result.type1 = Approach::Type::Vertex;
		}
		else
		{
			if ((vertex0.index1 < vertex1.index1) && (vertex0.index1 + 1 == vertex1.index1))
			{
				result.index1 = vertex0.index1;
			}
			else if ((vertex1.index1 < vertex0.index1) && (vertex1.index1 + 1 == vertex0.index1))
			{
				result.index1 = vertex1.index1;
			}
			else
			{
				result.index1 = std::max(vertex0.index1, vertex1.index1);
			}
			result.type1 = Approach::Type::Edge;
		}
	}
	break;

	case 3:
	{
		result.point0 = vertex0.point0 * (s * vertex0.u) + vertex1.point0 * (s * vertex1.u) + vertex2.point0 * (s * vertex2.u);
		result.point1 = vertex0.point1 * (s * vertex0.u) + vertex1.point1 * (s * vertex1.u) + vertex2.point1 * (s * vertex2.u);
		//result.point1 = result.point0;
	}
	break;

	default:
		assert(false);
		break;
	}

	result.distance = (result.point1 - result.point0).GetLength();

	result.iterations = iter;
	return std::move(result);
}

/*Spawner Defs*/
Spawner::Spawner(std::vector<std::unique_ptr<Asteroid>>& belt)
	:belt(belt), rng(std::random_device()())
{}

void Spawner::Update(const float dt, const Rect<float> cambox)
{
	//Cull distant asteroids
	for (auto& a : belt)
	{
		a->SetHistory();
		a->Update(dt);
	}

	const auto removecheck = [&](std::unique_ptr<Asteroid>& ast)
	{
		Vec<float> astpos = ast->GetPos();
		float astrad = ast->GetRadius();

		return astpos.Y - astrad > cambox.Y1 + cambox.height() / 2 ||
			astpos.Y + astrad < cambox.Y0 - cambox.height() / 2 ||
			astpos.X - astrad > cambox.X1 + cambox.width() / 2 ||
			astpos.X + astrad < cambox.X0 - cambox.width() / 2;
	};

	belt.erase(std::remove_if(belt.begin(), belt.end(),
		removecheck), belt.end());
	
	//Check Collisions
	CollCheck(dt);
	
	//Generate new asteroid, if appropriate
	GenTime -= dt;
	if (GenTime <= 0.0f)
	{
		if (int(belt.size()) < 16)
		{
			if (forcedAst != 0)
			{
				while (forcedAst > 0)
				{
					GenerateAsteroid(cambox);
					forcedAst--;
				}
			}

			GenerateAsteroid(cambox);

			std::uniform_real_distribution<float> nextAst(.3f, 2.0f);
			GenTime = nextAst(rng);
		}
	}
}

void Spawner::CollCheck(float dt)
{
	for (int i = 0; i < int(belt.size()); ++i)
	{
		for (int j = i + 1; j < belt.size(); ++j)
		{
			float dist2 = (belt[i]->GetPos() - belt[j]->GetPos()).GetLengthSq();
			float radi2 = belt[i]->GetRadius() + belt[j]->GetRadius();
			radi2 = radi2 * radi2;
			if (radi2 > dist2)
			{
				if (belt[i]->depth < Entity::CollDepth::FarField)
				{
					belt[i]->depth = Entity::CollDepth::FarField;
				}
				if (belt[j]->depth < Entity::CollDepth::FarField)
				{
					belt[j]->depth = Entity::CollDepth::FarField;
				}

				auto contact = std::make_unique<ActiveEdge>();
				float time = CollCheck(*belt[i], *belt[j], contact);

				if (time <= 1.0f)
				{
					if (belt[i]->depth < Entity::CollDepth::Collided)
					{
						belt[i]->depth = Entity::CollDepth::Collided;
					}
					if (belt[j]->depth < Entity::CollDepth::Collided)
					{
						belt[j]->depth = Entity::CollDepth::Collided;
					}

					float rewindTime = dt * (1 - time);

					belt[i]->Recoil(contact, *belt[j], rewindTime);
					belt[i]->SetHistory();
					belt[j]->SetHistory();
				}
			}
		}
	}
}

void Spawner::CollideShip(Entity& ship, float dt)
{
	{
		bool collision = false;
		for (int i = 0; i < int(belt.size()); ++i)
		{
			float dist2 = (belt[i]->GetPos() - ship.GetPos()).GetLengthSq();
			float radi2 = belt[i]->GetRadius() + ship.GetRadius();
			radi2 = radi2 * radi2;
			if (radi2 > dist2)
			{
				if (ship.depth < Entity::CollDepth::FarField)
				{
					ship.depth = Entity::CollDepth::FarField;
				}
				if (belt[i]->depth < Entity::CollDepth::FarField)
				{
					belt[i]->depth = Entity::CollDepth::FarField;
				}

				auto contact = std::make_unique<ActiveEdge>();
				//timeParam = 0 indicates last time step; = 1 indicates this time step. Most values expected between
				//timeParam = 2 indicates collision avoided in nearer analysis
				float timeParam = CollCheck(ship, *belt[i], contact);

				if (timeParam <= 1.0f)
				{
					auto ast = belt[i]->GetTransformedPrimitives();
					auto shipShape = ship.GetTransformedPrimitives();
					Approach debugTest = FindApproach(ast, shipShape);
					if (debugTest.index0 < 0)
						bool ProveAlgorithmHalts = true;

					if (ship.depth < Entity::CollDepth::Collided)
					{
						ship.depth = Entity::CollDepth::Collided;
					}
					if (belt[i]->depth < Entity::CollDepth::Collided)
					{
						belt[i]->depth = Entity::CollDepth::Collided;
					}

					float rewindTime = dt * (1 - timeParam);
					
					LastColl shipPremonition = ship.ReadHistory();
					LastColl astPremonition = belt[i]->ReadHistory();

					belt[i]->Recoil(contact, ship, rewindTime);
					
					ast = belt[i]->GetTransformedPrimitives();
					shipShape = ship.GetTransformedPrimitives();
					Approach debugTest2 = FindApproach(ast, shipShape);
					if (false && debugTest2.index0 < 0)
					{
						bool ProveAlgorithmHalts = true;
						ship.ResetHistory();
						belt[i]->ResetHistory();
						ship.ForgeHistory(shipPremonition);
						belt[i]->ForgeHistory(astPremonition);
					}
						
					belt[i]->SetHistory();
					ship.SetHistory();
					collision = true;
				}
			}
		}

		ship.didColl = collision;
	}
}

//More accurately, this should return collision info- (time and approach for simulating via Recoil or smth)
//Currently, t0 info is missing, how the eff to get..?
float Spawner::CollCheck(Entity& BodyA, Entity& BodyB, std::unique_ptr<ActiveEdge>& contactEdge)
{
	//time parameter
	float ToI = 2.f; //above 1.0f is "unitialized"

	LastColl BodyA0Hist = BodyA.ReadHistory();
	LastColl BodyB0Hist = BodyB.ReadHistory();

	std::vector<std::vector<Vec<float>>>& BodyA0Prims = BodyA0Hist.primitives;
	std::vector<std::vector<Vec<float>>>& BodyB0Prims = BodyB0Hist.primitives;

	std::vector<std::vector<Vec<float>>> BodyA1Prims = BodyA.GetTransformedPrimitives();
	std::vector<std::vector<Vec<float>>> BodyB1Prims = BodyB.GetTransformedPrimitives();

	bool* inRangeA = new bool[BodyA1Prims.size()];

	auto bbox = BodyB.GetBoundingBox();
	auto bd = BodyB.GetTransformedModel();
	
	//Mid-Field: Cull spectator primitives from consideration.
	for (int primInd = 0; primInd< BodyA1Prims.size(); ++primInd)
	{
		auto prim = BodyA1Prims[primInd];
		//Ad-hoc AABB for primitive- should have some functionality elsewhere?
		Rect<float> primRect = Rect<float>(prim[0], prim[0]);
		for  (auto v : prim)
		{
			if (v.X < primRect.X0)
			{
				primRect.X0 = v.X;
			}
			else if (v.X > primRect.X1)
			{
				primRect.X1 = v.X;
			}

			if (v.Y < primRect.Y0)
			{
				primRect.Y0 = v.Y;
			}
			else if (v.Y > primRect.Y1)
			{
				primRect.Y1 = v.Y;
			}
		}

		if (primRect.CollWith(BodyB.GetBoundingBox()))
		{
			inRangeA[primInd] = true;
		}
		else
		{
			inRangeA[primInd] = false;
		}
	}

	bool* inRangeB = new bool[BodyB1Prims.size()];

	//Mid-Field: Cull spectator primitives from consideration.
	for (int primInd = 0; primInd < BodyB1Prims.size(); ++primInd)
	{
		auto prim = BodyB1Prims[primInd];
		//Ad-hoc AABB for primitive- should have some functionality elsewhere?
		Rect<float> primRect = Rect<float>(prim[0], prim[0]);
		for (auto v : prim)
		{
			if (v.X < primRect.X0)
			{
				primRect.X0 = v.X;
			}
			else if (v.X > primRect.X1)
			{
				primRect.X1 = v.X;
			}

			if (v.Y < primRect.Y0)
			{
				primRect.Y0 = v.Y;
			}
			else if (v.Y > primRect.Y1)
			{
				primRect.Y1 = v.Y;
			}
		}

		if (primRect.CollWith(BodyA.GetBoundingBox()))
		{
			inRangeB[primInd] = true;
		}
		else
		{
			inRangeB[primInd] = false;
		}
	}

	for (int primI = 0; primI < BodyA1Prims.size(); ++primI)
	{
		if (inRangeA[primI])
		{
			for (int primJ = 0; primJ < BodyB1Prims.size(); ++primJ)
			{
				if (inRangeB[primJ])
				{
					if (BodyA.depth < Entity::CollDepth::MidField)
					{
						BodyA.depth = Entity::CollDepth::MidField;
					}
					if (BodyB.depth < Entity::CollDepth::MidField)
					{
						BodyB.depth = Entity::CollDepth::MidField;
					}

					ResolveNearField(BodyA0Prims[primI], BodyA1Prims[primI],
									BodyB0Prims[primJ], BodyB1Prims[primJ],
									ToI, contactEdge);
				}
			}
		}
	}

	delete[] inRangeA;
	delete[] inRangeB;
	return ToI;
}

void Spawner::ResolveNearField(std::vector<Vec<float>>& InitBodyA0, std::vector<Vec<float>>& InitBodyA1,
	std::vector<Vec<float>>& InitBodyB0, std::vector<Vec<float>>& InitBodyB1,
	float& t, std::unique_ptr<ActiveEdge>& foundEdge)
{
	float T0 = 0.0f;
	float T1 = 1.0f;
	float tolerance = 0.01; //IDK at all what this ought be...

	auto BodyA0 = InitBodyA0;
	auto BodyA1 = InitBodyA1;
	auto BodyB0 = InitBodyB0;
	auto BodyB1 = InitBodyB1;

	// Near-Mid Field
	// Quick check that these actually collide
	Rect<float> finalRectA = Rect<float>(BodyA1[0].X, BodyA1[0].Y, BodyA1[0].X, BodyA1[0].Y);
	Rect<float> finalRectB = Rect<float>(BodyB1[0].X, BodyB1[0].Y, BodyB1[0].X, BodyB1[0].Y);
	for (auto v : BodyA1)
	{
		if (v.X < finalRectA.X0)
		{
			finalRectA.X0 = v.X;
		}
		else if (v.X > finalRectA.X1)
		{
			finalRectA.X1 = v.X;
		}

		if (v.Y < finalRectA.Y0)
		{
			finalRectA.Y0 = v.Y;
		}
		else if (v.Y > finalRectA.Y1)
		{
			finalRectA.Y1 = v.Y;
		}
	}
	for (auto v : BodyB1)
	{
		if (v.X < finalRectB.X0)
		{
			finalRectB.X0 = v.X;
		}
		else if (v.X > finalRectB.X1)
		{
			finalRectB.X1 = v.X;
		}

		if (v.Y < finalRectB.Y0)
		{
			finalRectB.Y0 = v.Y;
		}
		else if (v.Y > finalRectB.Y1)
		{
			finalRectB.Y1 = v.Y;
		}
	}

	if (!finalRectA.CollWith(finalRectB))
	{
		return; //NO COLLISION (found in broad analysis)
	}



	Approach approach = FindApproach(BodyA0, BodyB0);
	if (approach.index0 < 0)
		bool crievritiem = true;
		//return;
	ActiveEdge currentEdge;

	if (approach.type0 == Approach::Type::Edge)
	{
		currentEdge = DeepestVsEdgeSolver(BodyA0[approach.index0], BodyA0[(approach.index0 + 1) % BodyA0.size()],
			BodyA1[approach.index0], BodyA1[(approach.index0 + 1) % BodyA1.size()],
			BodyB1
		);
		currentEdge.p0 = BodyB0[currentEdge.pInd];
		currentEdge.edgeIsA = true;

	}
	else if (approach.type1 == Approach::Type::Edge)
	{
		currentEdge = DeepestVsEdgeSolver(BodyB0[approach.index1], BodyB0[(approach.index1 + 1) % BodyB0.size()],
			BodyB1[approach.index1], BodyB1[(approach.index1 + 1) % BodyB1.size()],
			BodyA1
		);
		currentEdge.p0 = BodyA0[currentEdge.pInd];
		currentEdge.edgeIsA = false;
	}
	else
	{
		//constant line of separation
		currentEdge.n0 = approach.point1 - approach.point0;
		currentEdge.n1 = currentEdge.n0;
		currentEdge.planeConst0 = 0.0f;
		currentEdge.planeConst1 = 0.0f;
		//emulate p1-p0 as single point
		int deepestA = FindSupport(currentEdge.n0, BodyA1);
		int deepestB = FindSupport(-currentEdge.n0, BodyB1);
		currentEdge.p0 = BodyB0[deepestB] - BodyA0[deepestA];
		currentEdge.p1 = BodyB1[deepestB] - BodyA1[deepestA];;
		currentEdge.depth1 = currentEdge.n1.Dot(currentEdge.p1) - currentEdge.planeConst1;
		
		currentEdge.edgeIsA = false; // THIS COLLISION TYPE SHOULD NEVER BE THE END STATE. OH GOD, HOW DO I VERIFY THAT?
	}

	//shouldn't need this, but do...
	//if (currentEdge.depth1 > tolerance || approach.index0 < 0)
	
	if (currentEdge.depth1 > tolerance)
	{
		return;
	}
	if (currentEdge.depth1 < 0 && (currentEdge.n0.Dot(currentEdge.p0) - currentEdge.planeConst0 < -tolerance))
	{
		if (1 < t)
		{
			t = 1;
			*foundEdge = std::move(currentEdge);
		}
		return;
	}

	int iteration = 0;
	while (true)
	{
		++iteration;
		if (iteration > 200)
		{
			bool STOP = false;
			bool YouViolatedTheLaw = true;
		}
		
		if (abs(currentEdge.n0.Dot(currentEdge.p0) - currentEdge.planeConst0) < tolerance || approach.index0 <= 0)
		{
			//Collision Point Found at T0
			if (T0< t)
			{
				t = T0;
				*foundEdge = std::move(currentEdge);
			}
			return;
		}

		//init next iteration
		T1 = 1.f;
		BodyA1 = InitBodyA1;
		BodyB1 = InitBodyB1;

		int newiterations = 0;
		while (currentEdge.depth1 < -tolerance && T0 != 1.0f && approach.index0 >= 0)
		{
			++newiterations;
			if (newiterations > 200)
			{
				bool YOUVIOLATEDTHELAW = true;
				break;
			}
			if (currentEdge.depth1 > tolerance)
			{
				return; //NO COLLISION (found in near analysis)
			}
			float hold = FindRoot(currentEdge, T0, T1);
			if (isinf(hold))
			{
				bool Error = true;
			}
			T1 = hold;
			BodyA1 = BodyAtTime(InitBodyA0, InitBodyA1, T1);
			BodyB1 = BodyAtTime(InitBodyB0, InitBodyB1, T1);

			if (approach.type0 == Approach::Type::Edge)
			{
				currentEdge = DeepestVsEdgeSolver(BodyA0[approach.index0], BodyA0[(approach.index0 + 1) % BodyA0.size()],
					BodyA1[approach.index0], BodyA1[(approach.index0 + 1) % BodyA1.size()],
					BodyB1
				);
				currentEdge.p0 = BodyB0[currentEdge.pInd];
				currentEdge.edgeIsA = true;

			}
			else if (approach.type1 == Approach::Type::Edge)
			{
				currentEdge = DeepestVsEdgeSolver(BodyB0[approach.index1], BodyB0[(approach.index1 + 1) % BodyB0.size()],
					BodyB1[approach.index1], BodyB1[(approach.index1 + 1) % BodyB1.size()],
					BodyA1
				);
				currentEdge.p0 = BodyA0[currentEdge.pInd];
				currentEdge.edgeIsA = false;
			}
			else
			{
				//constant line of separation
				currentEdge.n0 = approach.point1 - approach.point0;
				currentEdge.n1 = currentEdge.n0;
				currentEdge.planeConst0 = 0.0f;
				currentEdge.planeConst1 = 0.0f;
				//emulate p1-p0 as single point
				int deepestA = FindSupport(currentEdge.n0, BodyA1);
				int deepestB = FindSupport(-currentEdge.n0, BodyB1);
				currentEdge.p0 = BodyB0[deepestB] - BodyA0[deepestA];
				currentEdge.p1 = BodyB1[deepestB] - BodyA1[deepestA];;
				currentEdge.depth1 = currentEdge.n1.Dot(currentEdge.p1);

				currentEdge.edgeIsA = false;
			}

		}
		T0 = T1;
		BodyA0 = BodyAtTime(InitBodyA0, InitBodyA1, T0);
		BodyB0 = BodyAtTime(InitBodyB0, InitBodyB1, T0);
		
		//Potentially new approach (should change if not within tolerance)
		approach = FindApproach(BodyA0, BodyB0);

		if (approach.type0 == Approach::Type::Edge)
		{
			currentEdge = DeepestVsEdgeSolver(BodyA0[approach.index0], BodyA0[(approach.index0 + 1) % BodyA0.size()],
				BodyA1[approach.index0], BodyA1[(approach.index0 + 1) % BodyA1.size()],
				BodyB1
			);
			currentEdge.p0 = BodyB0[currentEdge.pInd];
			currentEdge.edgeIsA = true;

		}
		else if (approach.type1 == Approach::Type::Edge)
		{
			currentEdge = DeepestVsEdgeSolver(BodyB0[approach.index1], BodyB0[(approach.index1 + 1) % BodyB0.size()],
				BodyB1[approach.index1], BodyB1[(approach.index1 + 1) % BodyB1.size()],
				BodyA1
			);
			currentEdge.p0 = BodyA0[currentEdge.pInd];
			currentEdge.edgeIsA = false;
		}
		else
		{
			//constant line of separation
			currentEdge.n0 = approach.point1 - approach.point0;
			currentEdge.n1 = currentEdge.n0;
			currentEdge.planeConst0 = 0.0f;
			currentEdge.planeConst1 = 0.0f;
			//emulate p1-p0 as single point
			int deepestA = FindSupport(currentEdge.n0, BodyA1);
			int deepestB = FindSupport(-currentEdge.n0, BodyB1);
			currentEdge.p0 = BodyB0[deepestB] - BodyA0[deepestA];
			currentEdge.p1 = BodyB1[deepestB] - BodyA1[deepestA];;
			currentEdge.depth1 = currentEdge.n0.Dot(currentEdge.p1);

			currentEdge.edgeIsA = false;
		}

		if (currentEdge.depth1 > tolerance)
		{
			return; //NO COLLISION (found in near analysis)
		}
	}
}

ActiveEdge Spawner::DeepestVsEdgeSolver(Vec<float> edgeI0, Vec<float> edgeJ0,
					Vec<float> edgeI1, Vec<float> edgeJ1,
					std::vector<Vec<float>> pointCloud
					)
{
	//Definition of planes: n(dot)p = const
	Vec<float> norm0 = edgeJ0 - edgeI0;
	float hold = norm0.X;
	norm0.X = norm0.Y;
	norm0.Y = -hold;
	float planeConst0 = norm0.Dot(edgeI0);

	Vec<float> norm1 = edgeJ1 - edgeI1;
	hold = norm1.X;
	norm1.X = norm1.Y;
	norm1.Y = -hold;
	float planeConst1 = norm1.Dot(edgeI1);

	float deepestFound = FLT_MAX;
	int deepestIndex = -1;
	for (int i = 0; i < pointCloud.size(); ++i)
	{
		float depth = norm1.Dot(pointCloud[i]) - planeConst1;
		if (depth < deepestFound)
		{
			deepestFound = depth;
			deepestIndex = i;
		}
	}

	ActiveEdge result;
	result.n0 = norm0;
	result.n1 = norm1;
	result.planeConst0 = planeConst0;
	result.planeConst1 = planeConst1;
	result.p1 = pointCloud[deepestIndex];
	result.pInd = deepestIndex;
	result.depth1 = deepestFound;

	return result;
}

float Spawner::FindRoot(ActiveEdge curEdge, float T0init, float T1init)
{
	float T0 = 0.0f;
	float T1 = 1.0f;
	float TDiff = T1init - T0init;
	//T1 may not initially be one, messing with interpolation
	float impactTime = T0;
	Vec<float> norm0 = curEdge.n0, norm1 = curEdge.n1;
	Vec<float> point0 = curEdge.p0, point1 = curEdge.p1;
	float planeConst0 = curEdge.planeConst0, planeConst1 = curEdge.planeConst1;

	float tolerance = 0.01; //IDK at all what this ought be...
	int max_iter = 200;

	for (int i = 0; i < max_iter; ++i)
	{
		float lastRoot;
		if (i % 2)
		{
			//false position root find iteration
			float separation0 = (norm0 + (norm1 - norm0) * T0).Dot(point0 + (point1 - point0) * T0) - planeConst0 - (planeConst1 - planeConst0) * T0;
			float separation1 = (norm0 + (norm1 - norm0) * T1).Dot(point0 + (point1 - point0) * T1) - planeConst0 - (planeConst1 - planeConst0) * T1;

			impactTime = separation0 * (T1 - T0) / (separation0 - separation1) + T0;
		}
		else
		{
			//bisection
			impactTime = (T0 + T1) * 0.5f;
		}

		if (impactTime < 0)
		{
			return 0.0f + T0init;
		}

		lastRoot = (norm0 + (norm1 - norm0) * impactTime).Dot(point0 + (point1 - point0) * impactTime) - planeConst0 - (planeConst1 - planeConst0) * impactTime;

		if (std::abs(lastRoot) < tolerance)
		{
			return impactTime*(TDiff) + T0init;
		}

		if (lastRoot > 0)
		{
			T0 = impactTime;
		}
		else
		{
			T1 = impactTime;
		}
	}

	return impactTime*TDiff + T0init; //Best Guess? Throw Error?
}

std::vector<Vec<float>> Spawner::BodyAtTime(std::vector<Vec<float>> Body0, std::vector<Vec<float>> Body1, float time)
{
	if (time == 0.0f)
	{
		return Body0;
	}
	
	if (time == 1.0f)
	{
		return Body1;
	}

	for (int i = 0; i < Body0.size(); ++i)
	{
		Body0[i] = Body0[i] + (Body1[i] - Body0[i]) * time;
	}

	return Body0;
}

void Spawner::GenerateAsteroid(const Rect<float> cambox)
{
	Rect<float> bounds = cambox;

	std::uniform_int_distribution<int> side(0, 3);
	std::uniform_real_distribution<float> xDist(bounds.X0, bounds.X1);
	std::uniform_real_distribution<float> yDist(bounds.Y0, bounds.Y1);
	std::uniform_real_distribution<float> vDist(-vRange, vRange);
	std::uniform_real_distribution<float> sDist(sMin, sMax);
	std::uniform_real_distribution<float> omgDist(-omgRange, omgRange);

	int spawnside = side(rng);

	if (spawnside == 0) //TOP
	{
		float scale = sDist(rng);
		Vec<float> pos = { xDist(rng), bounds.Y1 };
		Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
		vel.Y = -abs(vel.Y);
		float rot = omgDist(rng) / scale;
		belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
		belt[belt.size() - 1]->SetScale(scale);
	}

	if (spawnside == 1) //BOTTOM
	{
		float scale = sDist(rng);
		Vec<float> pos = { xDist(rng), bounds.Y0 };
		Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
		vel.Y = abs(vel.Y);
		float rot = omgDist(rng) / scale;
		belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
		belt[belt.size() - 1]->SetScale(scale);
	}

	if (spawnside == 2) //LEFT
	{
		float scale = sDist(rng);
		Vec<float> pos = { bounds.X0, yDist(rng) };
		Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
		vel.X = abs(vel.X);
		float rot = omgDist(rng) / scale;
		belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
		belt[belt.size() - 1]->SetScale(scale);
	}

	if (spawnside == 3) //RIGHT
	{
		float scale = sDist(rng);
		Vec<float> pos = { bounds.X1, yDist(rng) };
		Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
		vel.X = -abs(vel.X);
		float rot = omgDist(rng) / scale;
		belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
		belt[belt.size() - 1]->SetScale(scale);
	}

	int index = belt.size() - 1;

	//ensure no collision
	for (int i = 0; i < index; ++i)
	{
		float dist2 = (belt[i]->GetPos() - belt[index]->GetPos()).GetLengthSq();
		float radi2 = belt[i]->GetRadius() + belt[index]->GetRadius();
		radi2 = radi2 * radi2;
		if (radi2 < dist2)
		{
			std::vector<int> collider = belt[i]->CollWith(*belt[index]);
			if (collider.size() != 0)
			{
				belt.pop_back();
				break;
			}
		}
	}
}

int Spawner::FindSupport(const Vec<float>& d, const std::vector<Vec<float>>& model) const
{
	int furthestIndex = 0;
	float maxDist = model[0].Dot(d);
	for (int i = 1; i < int(model.size()); ++i)
	{
		float dist = model[i].Dot(d);
		if (dist > maxDist)
		{
			furthestIndex = i;
			maxDist = dist;
		}
	}

	return furthestIndex;
}

Approach Spawner::FindApproach(const std::vector<Vec<float>>& model0, const std::vector<Vec<float>>& model1) const
{
	//model and point presumed given in world space

	// Initial Simplex
	Simplex simplex;
	simplex.vertex0.index0 = 0;
	simplex.vertex0.index1 = 0;
	simplex.vertex0.point0 = model0[0];
	simplex.vertex0.point1 = model1[0];
	simplex.vertex0.point = simplex.vertex0.point1 - simplex.vertex0.point0;
	simplex.vertex0.u = 1.0f;
	simplex.count = 1;

	// Pointer to vertecies in simplex, acts like an array
	// Just don't be reckless and go out of bounds
	SimplexVertex* vertices = &simplex.vertex0;

	// Vertices of last simplex from previous iteration
	// Finding a repeat is primary termination criterion.
	int save0[3], save1[3];
	int saveCount = 0;

	// Loop evolving the simplex
	const int maxIters = 20;
	int iter = 0;
	while (iter < maxIters)
	{
		// Copy current verticies
		saveCount = simplex.count;
		for (int i = 0; i < saveCount; ++i)
		{
			save0[i] = vertices[i].index0;
			save1[i] = vertices[i].index1;
		}

		// Determine the closest point on the simplex and
		// remove unused vertices.
		switch (simplex.count)
		{
		case 1:
			break;

		case 2:
			simplex.Solve2();
			break;

		case 3:
			simplex.Solve3();
			break;
		default:
			assert(false);
		}

		// If interior to polygon, end
		if (simplex.count == 3)
		{
			break;
		}

		// Get search direction.
		Vec<float> d = simplex.GetSearchDirection();

		// Ensure the search direction non-zero.
		if (d.GetLengthSq() == 0.0f)
		{
			break;
		}

		// New Vertex to add to the Simplex
		SimplexVertex* newvert = vertices + simplex.count;
		newvert->index0 = FindSupport(-d, model0);
		newvert->index1 = FindSupport(d, model1);
		newvert->point0 = model0[newvert->index0];
		newvert->point1 = model1[newvert->index1];
		newvert->point = newvert->point1 - newvert->point0;

		++iter;

		// Primary escape criterion: repeated support point
		bool duplicate = false;
		for (int i = 0; i < saveCount; ++i)
		{
			if (newvert->index0 == save0[i] && newvert->index1 == save1[i])
			{
				duplicate = true;
				break;
			}
		}

		// If we found a duplicate support point we must exit to avoid cycling.
		if (duplicate)
		{
			break;
		}

		// New vertex is ok- Solve functions always reduce the dimension of the simplex, so this is safe
		++simplex.count;
	}

	return simplex.PrepareResult(iter, 0, 0);
}

Approach Spawner::FindApproach(const std::vector<std::vector<Vec<float>>>& modelList0, const std::vector<std::vector<Vec<float>>>& modelList1) const
{
	//Brutal force method (no mid phase filtering)

	//All cases for first element in model0
	Approach result = FindApproach(*modelList0.begin(), *modelList1.begin());

	for (auto it1 = ++modelList1.begin(); it1 != modelList1.end(); ++it1)
	{
		Approach test = FindApproach(*modelList0.begin(), *it1);

		if (test.distance < result.distance)
		{
			test.convex0 = 0;
			test.convex1 = std::distance(modelList1.begin(), it1);
			result = test;
			if (result.distance == 0.0f)
			{
				break;
			}
		}
	}

	//Remaining cases
	if (result.distance != 0.0f)
	{
		for (auto it0 = ++modelList0.begin(); it0 != modelList0.end(); ++it0)
		{
			for (auto it1 = modelList1.begin(); it1 != modelList1.end(); ++it1)
			{
				Approach test = FindApproach(*it0, *it1);

				if (test.distance < result.distance)
				{
					test.convex0 = std::distance(modelList0.begin(), it0);
					test.convex1 = std::distance(modelList1.begin(), it1);
					result = test;
					if (result.distance == 0.0f)
					{
						break;
					}
				}
			}
		}
	}
	return result;
}

float Spawner::FindMaxSeparation(int& edge, const std::vector<Vec<float>>& source, const std::vector<Vec<float>>& target) const
{
	int nSource = int(source.size());
	int nTarget = int(target.size());

	float maxSeparation = -FLT_MAX;

	for (int i = 0; i < nSource; ++i)
	{
		int i1 = (i + 1) % nSource;
		// source normal
		Vec<float> n = { source[i1].Y - source[i].Y, -source[i1].X + source[i].X };
		n = n.Norm();
		// Find deepest point for normal i.
		float si = FLT_MAX;
		for (int j = 0; j < nTarget; ++j)
		{
			float sij = n.Dot(target[j] - source[i]);
			if (sij < si)
			{
				si = sij;
			}
		}

		if (si > maxSeparation)
		{
			maxSeparation = si;
			edge = i;
		}
	}

	return maxSeparation;
}

int Spawner::FindIncidentEdge(const int edge, const std::vector<Vec<float>>& source, const std::vector<Vec<float>>& target) const
{
	int nverts = target.size();

	// Get the normal of the reference edge in poly2's frame.
	int edge1 = (edge + 1) % source.size();
	Vec<float> norm = {(source[edge1].Y - source[edge].Y), -(source[edge1].X - source[edge].X)};

	// Find the incident edge on poly2.
	int index = 0;
	float minDot = FLT_MAX;
	for (int i = 0; i < nverts; ++i)
	{
		int i1 = (i + 1) % nverts;
		float dot = norm.Dot( { (target[i1].Y - target[i].Y), - (target[i1].X - target[i].X) });
		if (dot < minDot)
		{
			minDot = dot;
			index = i;
		}
	}

	return index;
}


NearElements Spawner::GetNearestElements(const std::vector<Vec<float>>& model0,
										const std::vector<Vec<float>>& model1) const
{
	
	int edge0 = 0;
	int edge1 = 0;
	float separation0 = FindMaxSeparation(edge0, model0, model1);
	float separation1 = FindMaxSeparation(edge1, model1, model0);

	NearElements result;

	if (separation0 > separation1)
	{
		result.v0 = edge1;
		result.type0 = NearElements::Type::Edge;
		result.flip = true;
		result.v1 = FindIncidentEdge(result.v0, model1, model0);
		result.type1 = NearElements::Type::Vertex;
	}
	else
	{
		result.v0 = edge0;
		result.type0 = NearElements::Type::Edge;
		result.flip = false;
		result.v1 = FindIncidentEdge(result.v0, model0, model1);
		result.type1 = NearElements::Type::Vertex;
	}

	if(result.flip)
	{
		std::swap(result.v0, result.v1);
		std::swap(result.type0, result.type1);
		result.flip = false;
	}

	return result;
}