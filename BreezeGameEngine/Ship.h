#pragma once

#include "Entity.h"
#include "Shapes.h"

class Ship : public Entity
{
public:
	Ship(Vec<float> pos = { 0.0f, 0.0f }, Vec<float> vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::Cyan);

	void Thrust(float dt);
	void AThrust(float dt);
	void Update(float dt);
	void DriftDecay(float dt);

private:
	float thrust = 150.0f;
	float vdecay = 0.075f;
	float athrust = 2.0f;

	float rotdecay = 0.5f;

};