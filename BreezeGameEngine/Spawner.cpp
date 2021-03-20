#include<algorithm>
#include "Spawner.h"

void Spawner::Update(float dt, Rect<float> cambox)
{
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

			std::uniform_real_distribution<float> nextAst(1.0f, 5.0f);
			GenTime = nextAst(rng);
		}
	}

	CollCheck();
}

void Spawner::CollCheck()
{
	for (int i = 0; i < int(belt.size()); i++)
	{
		for (int j = i + 1; j < belt.size(); j++)
		{
			float dist2 = (belt[i]->GetPos() - belt[j]->GetPos()).GetLengthSq();
			float radi2 = belt[i]->GetRadius() + belt[j]->GetRadius();
			radi2 = radi2 * radi2;
			if (radi2 > dist2)
			{
				if (belt[i]->CollWith(*belt[j]))
				{
					belt[i]->Recoil(*belt[j]);
				}
			}
		}
	}
}

void Spawner::GenerateAsteroid(Rect<float> cambox)
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

	for (int i = 0; i < index; i++)
	{
		float dist2 = (belt[i]->GetPos() - belt[index]->GetPos()).GetLengthSq();
		float radi2 = belt[i]->GetRadius() + belt[index]->GetRadius();
		radi2 = radi2 * radi2;
		if (radi2 > dist2)
		{
			if (belt[i]->CollWith(*belt[index]))
			{
				belt.pop_back();
				break;
			}
		}
	}
}
