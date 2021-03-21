#define NOMINMAX // Needed to make min work
#include<algorithm>
#include "Ship.h"

Ship::Ship(Vec<float> pos, Vec<float> vel, float rot, Color c)
	:Entity(Shapes::Ship(), pos, vel, rot, c)
{
}

void Ship::Thrust(float dt)
{
	if (vel.GetLengthSq() < vmax * vmax)
	{
		vel += GetHeading() * thrust * dt;
	}
	else
	{
		Vec<float> vnorm = vel / vmax;
		Vec<float> vtang = { -vnorm.Y, vnorm.X };

		float alignment = vtang.Dot(GetHeading());
		if (alignment < 0.0f)
		{
			vtang *= -1;
			alignment *= -1;
		}

		vel += (vtang - vnorm) * thrust * dt * alignment;
	}
}

void Ship::AThrust(float dt)
{
	rot = std::min(rot + athrust * dt, rotmax);
}

void Ship::Update(float dt)
{
	TranslateBy(vel * dt);
	RotBy(rot * dt);
	DriftDecay(dt);
}

void Ship::DriftDecay(float dt)
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
	if (abs(rot) < dth)
	{
		rot = 0.0f;
	}
	else if (signbit(rot))
	{
		rot += dth;
	}
	else
	{
		rot -= dth;
	}
}