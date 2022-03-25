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
#define NOMINMAX
#include<algorithm>
#include<cassert>

#include "Spawner.h"

Spawner::Spawner(std::vector<std::unique_ptr<Asteroid>>& belt)
	:belt(belt), rng(std::random_device()())
{}

void Spawner::Update(const float dt, const Rect<float>& cambox)
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
			GenerateAsteroid(cambox);

			std::uniform_real_distribution<float> nextAst(.3f, 2.0f);
			GenTime = nextAst(rng);
		}
	}
}

void Spawner::Initialize(const Rect<float>& cambox)
{
	for (int i=0; i<forcedAst; ++i)
	{
		GenerateAsteroid(cambox);
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
