#pragma once

#include "Entity.h"
#include "Shapes.h"

class Asteroid : public Entity
{
public:
	Asteroid(Vec<float> pos = { 0.0f, 0.0f }, Vec<float> vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::LightGrey)
		:Entity(Shapes::Asteroid(), pos, vel, rot, c)
	{
	}

	void Update(const float dt)
	{
		TranslateBy(vel * dt);
		RotBy(rot * dt);
	}
};