#pragma once

#include<memory>
#include <random>
#include "Asteroid.h"

class Spawner
{
public:
	Spawner(std::vector<std::unique_ptr<Asteroid>>& belt)
		:belt(belt), rng(std::random_device()())
	{}

	void Update(float dt, Rect<float> cambox);

	void CollideShip(Entity& ship);

private:
	void CollCheck();
	void GenerateAsteroid(Rect<float> cambox);

	std::vector<std::unique_ptr<Asteroid>>& belt;
	float GenTime = 0.0f;
	std::mt19937 rng;

	float vRange = 300.0f;
	float sMin = 0.75f;
	float sMax = 5.0f;
	float omgRange = 5.0f;

	int forcedAst = 5;
};