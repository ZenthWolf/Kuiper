#include<algorithm>
#include<cassert>
#include "Spawner.h"

/*Simplex Defs*/

/*
In Polygon-Polygon collisions, this "Simplex" is that of
the Minkowski Difference generated polygon
*/

//Search is relative, so normalizing not important
Vec<float> Simplex::GetSearchDirection(const Vec<float>& Q) const
{
	switch (count)
	{
	case 1:
		return Q-vertex0.point;

	case 2:
	{
		Vec<float> edge = vertex1.point - vertex0.point;
		float sgn = edge.Cross(Q-vertex0.point);
		if (sgn > 0.0f)
		{
			// Q is left of edge.
			return Vec<float>{-edge.Y, edge.X};
		}
		else
		{
			// Q is right of edge.
			return Vec<float>{edge.Y, -edge.X};
		}
	}

	default:
		assert(false);
		return Vec<float>(0.0f, 0.0f);
	}
}

Vec<float> Simplex::GetClosestPoint() const
{
	//This version valid while not considering polygon-polygon approaches
	float factor = 1.0f / divisor;

	switch (count)
	{
	case 1:
		return vertex0.point;

	case 2:
	{
		float s = 1.0f / divisor;
		return vertex0.point * (s * vertex0.u) +  vertex1.point * (s * vertex1.u);
	}
	break;

	case 3:
	{
		float s = 1.0f / divisor;
		return vertex0.point * (s * vertex0.u) + vertex1.point * (s * vertex1.u) + vertex2.point * (s * vertex2.u);
	}
	break;

	default:
		assert(false);
		return Vec<float>();
	}
}

//Find closest point on this 1-Plex to a pnt
void Simplex::Solve2(const Vec<float>& Q)
{
	Vec<float> A = vertex0.point;
	Vec<float> B = vertex1.point;

	//Sign flips to ensure positive results
	float u = (Q - B).Dot(A - B);
	float v = (Q - A).Dot(B - A);

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
void Simplex::Solve3(const Vec<float>& Q)
{
	Vec<float> A = vertex0.point;
	Vec<float> B = vertex1.point;
	Vec<float> C = vertex2.point;

	// Unnormalized edge barycentric coordinate
	float uAB = (Q - B).Dot(A - B);
	float vAB = (Q - A).Dot(B - A);

	float uBC = (Q - C).Dot(B - C);
	float vBC = (Q - B).Dot(C - B);

	float uCA = (Q - A).Dot(C - A);
	float vCA = (Q - C).Dot(A - C);

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
	float uQBC = (B - Q).Cross(C - Q);
	float vAQC = (C - Q).Cross(A - Q);
	float wABQ = (A - Q).Cross(B - Q);

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

/*Spawner Defs*/
Spawner::Spawner(std::vector<std::unique_ptr<Asteroid>>& belt)
	:belt(belt), rng(std::random_device()())
{}

void Spawner::Update(const float dt, const Rect<float> cambox)
{
	//Cull distant asteroids
	for (auto& a : belt)
	{
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
	CollCheck();
	
	//Generate new asteroid, if appropriate
	GenTime -= dt;
	if (GenTime <= 0.0f)
	{
		if (int(belt.size()) < 15)
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

void Spawner::CollCheck()
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
				std::vector<int> collider = belt[i]->CollWith(*belt[j]);

				if (collider.size() != 0)
				{
					belt[i]->Recoil(collider, *belt[j]);
					belt[i]->SetHistory();
					belt[j]->SetHistory();
				}

				else
				{
					std::vector<int> collider = belt[j]->CollWith(*belt[i]);
					if (collider.size() != 0)
					{
						belt[j]->Recoil(collider, *belt[i]);
						belt[i]->SetHistory();
						belt[j]->SetHistory();
					}
				}
			}
		}
	}
}

void Spawner::CollideShip(Entity& ship)
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
				std::vector<int> collider = belt[i]->CollWith(ship);

				if (collider.size() != 0)
				{
					belt[i]->Recoil(collider, ship);
					belt[i]->SetHistory();
					ship.SetHistory();
					collision = true;
				}
				else
				{
					std::vector<int> collider = ship.CollWith(*belt[i]);
					if (collider.size() != 0)
					{
						ship.Recoil(collider, *belt[i]);
						belt[i]->SetHistory();
						ship.SetHistory();
						collision = true;
					}
				}
			}
		}

		ship.didColl = collision;
	}
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
		if (radi2 > dist2)
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

Approach Spawner::FindApproach(Vec<float> pnt, std::vector<Vec<float>> model) const
{
	//model and point presumed given in world space

	// Initial Simplex
	Simplex simplex;
	simplex.vertex0.index = 0;
	simplex.vertex0.point = model[0];
	simplex.vertex0.u = 1.0f;
	simplex.count = 1;

	// Pointer to vertecies in simplex, acts like an array
	// Just don't be reckless and go out of bounds
	SimplexVertex* vertices = &simplex.vertex0;

	// Vertices of last simplex from previous iteration
	// Finding a repeat is primary termination criterion.
	int save[3];
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
			save[i] = vertices[i].index;
		}

		// Determine the closest point on the simplex and
		// remove unused vertices.
		switch (simplex.count)
		{
		case 1:
			break;

		case 2:
			simplex.Solve2(pnt);
			break;

		case 3:
			simplex.Solve3(pnt);
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
		Vec<float> d = simplex.GetSearchDirection(pnt);

		// Ensure the search direction non-zero.
		if (d.GetLengthSq() == 0.0f)
		{
			break;
		}

		// New Vertex to add to the Simplex
		SimplexVertex* newvert = vertices + simplex.count;
		newvert->index = FindSupport(d, model);
		newvert->point = model[newvert->index];

		++iter;

		// Primary escape criterion: repeated support point
		bool duplicate = false;
		for (int i = 0; i < saveCount; ++i)
		{
			if (newvert->index == save[i])
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

	// Prepare output.
	Approach result;
	result.point1 = pnt;
	result.point2 = simplex.GetClosestPoint();
	result.distance = (result.point1 - result.point2).GetLength();
	result.iterations = iter;

	return result;
}

Approach Spawner::FindApproach(Vec<float> pnt, std::list<std::vector<Vec<float>>> modelList) const
{
	Approach result = FindApproach(pnt, *modelList.begin());
	for (auto it = ++modelList.begin(); it != modelList.end(); ++it)
	{
		Approach test = FindApproach(pnt, *it);

		if (test.distance < result.distance)
		{
			result = test;
			if (result.distance == 0.0f)
			{
				break;
			}
		}
	}
	return result;
}
