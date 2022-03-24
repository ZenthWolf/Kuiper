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
	const int maxIters = 10;
	int iters = 0;
	while (iters < maxIters)
	{
		++iters;

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
		bool accepted = true;
		for (int i = 0; i < index; ++i)
		{
			float dist2 = (belt[i]->GetPos() - belt[index]->GetPos()).GetLengthSq();
			float radi2 = belt[i]->GetRadius() + belt[index]->GetRadius();
			radi2 = radi2 * radi2;
			if (radi2 > dist2)
			{
				accepted = false;
				belt.pop_back();
				break;
			}
		}
		if (accepted)
			break;
	}
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