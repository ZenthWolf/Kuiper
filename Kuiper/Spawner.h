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

#include<memory>
#include <random>
#include "Asteroid.h"

class Spawner
{
public:
	Spawner(std::vector<std::unique_ptr<Asteroid>>& belt);
	void Update(const float dt, const Rect<float>& cambox);
	void Initialize(const Rect<float>& cambox);
private:
	void GenerateAsteroid(const Rect<float> cambox);

	std::vector<std::unique_ptr<Asteroid>>& belt;
	float GenTime = 0.0f;
	std::mt19937 rng;

	const float vRange = 300.0f;
	const float sMin = 0.75f;
	const float sMax = 5.0f;
	const float omgRange = 5.0f;

	const int forcedAst = 5;
};