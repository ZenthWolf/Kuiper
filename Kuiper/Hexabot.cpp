#include "Hexabot.h"
#include "Shapes.h"

Hexabot::Hexabot(Entity& targ, Vec<float> pos, Vec<float> vel, float rot, Color c)
	:Entity(Shapes::Hexabot(), pos, vel, rot, c), target(targ)
{
	vmax = 50.0f;
	scale = 10.0f;
}

void Hexabot::Thrust(const float dt)
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

void Hexabot::AThrust(const float dt)
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

void Hexabot::Update(const float dt)
{
	Vec<float> toTarget = (target.GetPos() - pos).Norm();
	if(toTarget.Dot(GetHeading()) > 0.0f)
		Thrust(dt * toTarget.Dot(GetHeading()));
	AThrust(-dt * toTarget.Cross(GetHeading()));

	alertStaleModel();
	TranslateBy(vel * dt);
	RotBy(angvel * dt);
}
