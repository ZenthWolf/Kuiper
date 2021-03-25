#include<algorithm>
#include "Spawner.h"

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
	for (int i = 0; i < int(belt.size()); i++)
	{
		for (int j = i + 1; j < belt.size(); j++)
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
				}

				else
				{
					std::vector<int> collider = belt[j]->CollWith(*belt[i]);
					if (collider.size() != 0)
					{
						belt[j]->Recoil(collider, *belt[i]);
					}
				}
			}
		}
	}
}

void Spawner::CollideShip(Entity& ship)
{
	{
		for (int i = 0; i < int(belt.size()); i++)
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
				}
				else
				{
					std::vector<int> collider = ship.CollWith(*belt[i]);
					if (collider.size() != 0)
					{
						ship.Recoil(collider, *belt[i]);
					}
				}
			}
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
	for (int i = 0; i < index; i++)
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
