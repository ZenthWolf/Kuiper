#include "ColliderManager.h"

void ColliderManager::DoCollisions(const float dt, const bool collship) const
{
	for (int i = 0; i < int(belt.size()); ++i)
	{
		for (int j = i + 1; j < belt.size(); ++j)
			DoBinaryCollision(*belt[i], *belt[j], dt);

		if(collship)
			DoBinaryCollision(*belt[i], ship, dt);
	}
}

void ColliderManager::DoBinaryCollision(Entity& BodyA, Entity& BodyB, const float dt) const
{
	float dist2 = (BodyA.GetPos() - BodyB.GetPos()).GetLengthSq();
	float radi2 = BodyA.GetRadius() + BodyB.GetRadius();
	radi2 = radi2 * radi2;
	if (radi2 > dist2)
	{
		if (BodyA.depth < Entity::CollDepth::FarField)
		{
			BodyA.depth = Entity::CollDepth::FarField;
		}
		if (BodyB.depth < Entity::CollDepth::FarField)
		{
			BodyB.depth = Entity::CollDepth::FarField;
		}

		ActiveEdge contact;
		//timeParam = 0 indicates last time step; = 1 indicates this time step. Most values expected between
		//timeParam = 2 indicates collision avoided in nearer analysis
		float time = ResolveMidField(BodyA, BodyB, contact);

		if (time <= 1.0f)
		{
			if (BodyA.depth < Entity::CollDepth::Collided)
			{
				BodyA.depth = Entity::CollDepth::Collided;
			}
			if (BodyB.depth < Entity::CollDepth::Collided)
			{
				BodyB.depth = Entity::CollDepth::Collided;
			}

			float rewindTime = dt * (1 - time);

			BodyB.Recoil(contact, BodyA, rewindTime);
			BodyA.SetHistory();
			BodyB.SetHistory();
		}
	}
}

/// <summary>
/// Checks for potential collision in midfield- calls near field resolver if collision expected
/// Returning 2.0f indicates no collision happened.
/// </summary>
/// <param name="outContactEdge">Secondary return</param>
/// <returns>Parameterized time in range [0,1], outContactEdge filled with collision information</returns>
float ColliderManager::ResolveMidField(Entity& BodyA, Entity& BodyB, ActiveEdge& outContactEdge) const
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

	//Mid-Field: Cull spectator primitives from consideration.
	for (int primInd = 0; primInd < BodyA1Prims.size(); ++primInd)
	{
		Rect<float> primRect = Shapes::BoundingBox(BodyA1Prims[primInd]);

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
		Rect<float> primRect = Shapes::BoundingBox(BodyB1Prims[primInd]);

		if (primRect.CollWith(BodyA.GetBoundingBox()))
		{
			inRangeB[primInd] = true;
		}
		else
		{
			inRangeB[primInd] = false;
		}
	}

	for (int i = 0; i < BodyA1Prims.size(); ++i)
	{
		if (inRangeA[i])
		{
			for (int j = 0; j < BodyB1Prims.size(); ++j)
			{
				if (inRangeB[j])
				{
					if (BodyA.depth < Entity::CollDepth::MidField)
					{
						BodyA.depth = Entity::CollDepth::MidField;
					}
					if (BodyB.depth < Entity::CollDepth::MidField)
					{
						BodyB.depth = Entity::CollDepth::MidField;
					}

					ResolveNearField(BodyA0Prims[i], BodyA1Prims[i],
						BodyB0Prims[j], BodyB1Prims[j],
						ToI, outContactEdge);
				}
			}
		}
	}

	delete[] inRangeA;
	delete[] inRangeB;
	return ToI;
}

void ColliderManager::ResolveNearField(const std::vector<Vec<float>>& InitBodyA0, const std::vector<Vec<float>>& InitBodyA1, 
									const std::vector<Vec<float>>& InitBodyB0, const std::vector<Vec<float>>& InitBodyB1, 
									float& t, ActiveEdge& outfoundEdge) const
{
	//Time parameter between this and last step
	float T0 = 0.0f;
	float T1 = 1.0f;

	auto BodyA0 = InitBodyA0;
	auto BodyA1 = InitBodyA1;
	auto BodyB0 = InitBodyB0;
	auto BodyB1 = InitBodyB1;

	// Near-Mid Field
	// Check that these elements actually collide
	Rect<float> finalRectA = Shapes::BoundingBox(BodyA1);
	Rect<float> finalRectB = Shapes::BoundingBox(BodyB1);

	if (!finalRectA.CollWith(finalRectB))
	{
		return; //NO COLLISION (found in broad analysis)
	}

	/*********************************************************************************/
	/*********************************************************************************/
	/*****************************CONTINUE********************************************/
	/*******************************FROM**********************************************/
	/*******************************HERE**********************************************/
	/*********************************************************************************/
	/*********************************************************************************/

	Approach approach = Shapes::FindApproach(BodyA0, BodyB0);
	if (approach.index0 < 0)
		return ResolveDiscreteCollision(InitBodyA0, InitBodyB0, t, outfoundEdge);

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
		int deepestA = Shapes::FindSupport(currentEdge.n0, BodyA1);
		int deepestB = Shapes::FindSupport(-currentEdge.n0, BodyB1);
		currentEdge.p0 = BodyB0[deepestB] - BodyA0[deepestA];
		currentEdge.p1 = BodyB1[deepestB] - BodyA1[deepestA];;
		currentEdge.depth1 = currentEdge.n1.Dot(currentEdge.p1) - currentEdge.planeConst1;

		currentEdge.edgeIsA = false; // THIS COLLISION TYPE SHOULD NEVER BE THE END STATE. OH GOD, HOW DO I VERIFY THAT?
	}

	//shouldn't need this, but do...
	//if (currentEdge.depth1 > tolerance || approach.index0 < 0)
	//TODO VERIFY UTILITY
	if (currentEdge.depth1 > tolerance)
	{
		return;
	}
	//TODO VERIFY UTILITY
	if (currentEdge.depth1 < 0 && (currentEdge.n0.Dot(currentEdge.p0) - currentEdge.planeConst0 < -tolerance))
	{
		if (1 < t)
		{
			t = 1;
			outfoundEdge = std::move(currentEdge);
		}
		return;
	}

	int iteration = 0;
	while (true)
	{
		++iteration;

		//Primary Exit
		if (abs(currentEdge.n0.Dot(currentEdge.p0) - currentEdge.planeConst0) < tolerance || approach.index0 <= 0)
		{
			//Collision Point Found at T0
			if (T0 < t)
			{
				t = T0;
				outfoundEdge = std::move(currentEdge);
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

			if (currentEdge.depth1 > tolerance)
				return; //NO COLLISION (found in near analysis)

			T1 = FindRoot(currentEdge, T0, T1);

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
				int deepestA = Shapes::FindSupport(currentEdge.n0, BodyA1);
				int deepestB = Shapes::FindSupport(-currentEdge.n0, BodyB1);
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
		approach = Shapes::FindApproach(BodyA0, BodyB0);

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
			int deepestA = Shapes::FindSupport(currentEdge.n0, BodyA1);
			int deepestB = Shapes::FindSupport(-currentEdge.n0, BodyB1);
			currentEdge.p0 = BodyB0[deepestB] - BodyA0[deepestA];
			currentEdge.p1 = BodyB1[deepestB] - BodyA1[deepestA];;
			currentEdge.depth1 = currentEdge.n0.Dot(currentEdge.p1);

			currentEdge.edgeIsA = false;
		}

		if (currentEdge.depth1 > tolerance)
			return; //NO COLLISION (found in near analysis)
	}
}

/// <summary>
/// Fall back routine if continuous collision fails in the
/// event that collision is present at initial time.
/// </summary>
void ColliderManager::ResolveDiscreteCollision(const std::vector<Vec<float>>& BodyA, const std::vector<Vec<float>>& BodyB, float& t, ActiveEdge& outfoundEdge) const
{
	float shallowestMove = FLT_MAX;

	ActiveEdge currentEdge = DeepestVsEdgeSolver(BodyA[0], BodyA[1], BodyA[0], BodyA[1], BodyB);

	if (signbit(currentEdge.depth1))
	{
		float normalization = currentEdge.n0.GetLengthSq();
		float depth = currentEdge.depth1 * currentEdge.depth1 / normalization;
	}


	for (int edge = 1; edge < BodyA.size(); ++edge)
	{
		currentEdge = DeepestVsEdgeSolver(BodyA[edge], BodyA[(edge + 1) % BodyA.size()], BodyA[edge], BodyA[(edge + 1) % BodyA.size()], BodyB);

		if (signbit(currentEdge.depth1))
		{
			float normalization = currentEdge.n0.GetLengthSq();
			float depth = currentEdge.depth1 * currentEdge.depth1 / normalization;
			if (depth < shallowestMove)
			{
				shallowestMove = depth;

				outfoundEdge = std::move(currentEdge);
			}
		}
	}

	bool edgeIsA = true;

	for (int edge = 1; edge < BodyB.size(); ++edge)
	{
		currentEdge = DeepestVsEdgeSolver(BodyB[edge], BodyB[(edge + 1) % BodyB.size()], BodyB[edge], BodyB[(edge + 1) % BodyB.size()], BodyA);

		if (signbit(currentEdge.depth1))
		{
			float normalization = currentEdge.n0.GetLengthSq();
			float depth = currentEdge.depth1 * currentEdge.depth1 / normalization;
			if (depth < shallowestMove)
			{
				edgeIsA = false;
				shallowestMove = depth;
				outfoundEdge = std::move(currentEdge);
			}
		}
	}

	outfoundEdge.p0 = outfoundEdge.p1;
	outfoundEdge.edgeIsA = edgeIsA;
	outfoundEdge.discreteCollision = true;
	outfoundEdge.depth1 = sqrt(shallowestMove);
	t = 0.0f;
}

ActiveEdge ColliderManager::DeepestVsEdgeSolver(const Vec<float>& edgeI0, const Vec<float>& edgeJ0,
												const Vec<float>& edgeI1, const Vec<float>& edgeJ1,
												const std::vector<Vec<float>>& pointCloud) const
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

	return std::move(result);
}

float ColliderManager::FindRoot(const ActiveEdge& curEdge, const float& T0init, const float& T1init) const
{
	//parameterizes between the parameterized times- it makes sense, I promise
	float T0 = 0.0f;
	float T1 = 1.0f;
	float TDiff = T1init - T0init;

	float impactTime = T0;
	Vec<float> norm0 = curEdge.n0, norm1 = curEdge.n1;
	Vec<float> point0 = curEdge.p0, point1 = curEdge.p1;
	float planeConst0 = curEdge.planeConst0, planeConst1 = curEdge.planeConst1;

	const int MAX_ITER = 200;

	for (int i = 0; i < MAX_ITER; ++i)
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
			return impactTime * (TDiff)+T0init;
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

	return impactTime * TDiff + T0init; //Best Guess? Throw Error?
}

std::vector<Vec<float>> ColliderManager::BodyAtTime(const std::vector<Vec<float>>& Body0, 
													const std::vector<Vec<float>>& Body1, const float timeParam) const
{
	std::vector<Vec<float>> lerpedBody;
	lerpedBody.reserve(Body0.size());
	if (timeParam == 0.0f)
	{
		for (int i = 0; i < Body0.size(); ++i)
		{
			lerpedBody.emplace_back(Body0[i]);
		}
	}

	else if (timeParam == 1.0f)
	{
		for (int i = 0; i < Body0.size(); ++i)
		{
			lerpedBody.emplace_back(Body1[i]);
		}
	}

	else
	{
		for (int i = 0; i < Body0.size(); ++i)
		{
			lerpedBody.emplace_back(Body0[i] + (Body1[i] - Body0[i]) * timeParam);
		}
	}

	return std::move(lerpedBody);
}
