#pragma once

#include "Entity.h"
#include "Shapes.h"

class Ship : public Entity
{
public:
	Ship(Vec<float> pos = { 0.0f, 0.0f }, Vec<float> vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::Cyan)
		:Entity(Shapes::Ship(), pos, vel, rot, c)
	{
		modelburn = Shapes::ShipBurn();
	}

	void Thrust(float dt)
	{
		Vec<float> nvel = vel + GetHeading() * thrust;
		if (nvel.GetLengthSq() < vmax*vmax)
		{
			vel = nvel;
		}
	}

	void AThrust(float dt)
	{
		rot = std::min(rot + athrust * dt, rotmax);
	}

	void Update(float dt)
	{
		TranslateBy(vel * dt);
		RotBy(rot * dt);
		DriftDecay(dt);
	}

	void DriftDecay(float dt)
	{
		Vec<float> nvel = vel - vel.Norm() * vdecay;
		if (nvel.Dot(vel) <= 0.0f)
		{
			vel = { 0.0f, 0.0f };
		}
		else
		{
			vel = nvel;
		}

		float dth = dt * rotdecay;
		if(abs(rot) < dth)
		{
			rot = 0.0f;
		}
		else if(signbit(rot))
		{
			rot += dth;
		}
		else
		{
			rot -= dth;
		}
	}

private:
	std::vector<Vec<float>> modelburn;
	

	float thrust = .5f;
	float vdecay = 0.075f;
	float athrust = 2.0f;

	float rotdecay = 0.5f;

	bool burn = false;
	bool rburn = false;
};