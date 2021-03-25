#pragma once

#include "Entity.h"
#include "Shapes.h"

class Ship : public Entity
{
public:
	Ship(Vec<float> pos = { 0.0f, 0.0f }, Vec<float> vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::Cyan);

	void Thrust(const float dt);
	void AThrust(const float dt);
	void Update(const float dt);
	void DriftDecay(const float dt);

private:
	float thrust = 150.0f;
	float vdecay = 7.5f;
	float athrust = 2.0f;

	float rotdecay = 0.5f;

};