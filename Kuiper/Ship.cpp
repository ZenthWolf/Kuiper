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
#define NOMINMAX // Needed to make min work
#include<algorithm>
#include "Ship.h"

Ship::Ship(Vec<float> pos, Vec<float> vel, float rot, Color c)
	:Entity(Shapes::Ship(), pos, vel, rot, c)
{
}

void Ship::Thrust(const float dt)
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

void Ship::AThrust(const float dt)
{
	if (abs(angvel) < angvelMax)
	{
		angvel += athrust * dt;
	}
	else if (signbit(angvel) != signbit(dt))
	{
		angvel += athrust * dt;
	}
}

void Ship::Update(const float dt)
{
	alertStaleModel();
	TranslateBy(vel * dt);
	RotBy(angvel * dt);
}

void Ship::DriftDecay(const float dt)
{
	alertStaleModel();
	Vec<float> nvel = vel - vel.Norm() * vdecay * dt;

	if (nvel.Dot(vel) <= 0.0f)
	{
		vel = { 0.0f, 0.0f };
	}
	else
	{
		vel = nvel;
	}

	float dth = rotdecay * dt;
	if (abs(angvel) < dth)
	{
		angvel = 0.0f;
	}
	else if (signbit(angvel))
	{
		angvel += dth;
	}
	else
	{
		angvel -= dth;
	}
}