#pragma once
#include "Entity.h"

class Hexabot : public Entity
{
public:
	Hexabot(Entity& targ, Vec<float> pos = { 0.0f, 0.0f }, Vec<float> vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::LightGrey);

	void Thrust(const float dt);
	void AThrust(const float dt);
	void Update(const float dt);

private:
	float thrust = 50;
	float athrust = 200.0f;

	const Entity& target;
	bool escaping = false;
};