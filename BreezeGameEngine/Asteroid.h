#pragma once
#pragma once

#include "Entity.h"
#include "Shapes.h"

class Asteroid : public Entity
{
public:
	Asteroid(Vec<float> pos = { 0.0f, 0.0f }, Vec<float> vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::LightGray)
		:Entity(Shapes::Asteroid(), pos, c), vel(vel), rot(rot)
	{
	}

	Vec<float> GetVel() const
	{
		return vel;
	}

	void SetVel(Vec<float> newvel)
	{
		vel = newvel;
	}

	void Update(float dt)
	{
		TranslateBy(vel * dt);
		RotBy(rot * dt);
	}

private:
	Vec<float> vel;
	float rot;
};