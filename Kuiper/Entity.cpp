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
#define NOMINMAX // I just want std::min to work, Windows...

#include<algorithm>
#include "Shapes.h"
#include "Entity.h"

#include "Spawner.h"

#define BREEZE_COLLIDER_EXCEPTION( note ) Entity::ColliderException( note,_CRT_WIDE(__FILE__),__LINE__ )

Entity::Entity(std::vector<Vec<float>> modl, const Vec<float>& pos = { 0.0f, 0.0f }, const Vec<float>& vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::White)
	:model(std::move(modl)), pos(pos), vel(vel), rot(rot), c(c)
{
	Vec<float> CoM = model[0];
	for (int i = 1; i < model.size(); ++i)
	{
		CoM += model[i];
	}
	CoM *= 1 / ((float)model.size());
	for (auto v : model)
	{
		float vrad = (v-CoM).GetLengthSq();
		if (vrad > boundingrad)
		{
			boundingrad = vrad;
		}
	}

	boundingrad = sqrtf(boundingrad);

	modelprimitives = Shapes::ConvexSeparator(model);

	/*
	for (auto m : modelprimitives)
	{
		for (auto v : m)
		{
			float vrad = v.GetLengthSq();
			if (vrad > boundingrad)
			{
				boundingrad = vrad;
			}
		}
	}
	*/
	SetHistory();
}




const Vec<float>& Entity::GetPos() const
{
	return pos;
}

void Entity::SetPos(const Vec<float> newpos)
{
	pos = newpos;
}

void Entity::TranslateBy(const Vec<float> dpos)
{
	pos += dpos;
}

void Entity::SetScale(const float s)
{
	scale = s;
	boundingrad *= s;
}

float Entity::GetScale() const
{
	return scale;
}

float Entity::GetRadius() const
{
	return boundingrad;
}

void Entity::SetHeading(const float th)
{
	heading = th;
	while (heading > (2 * 3.1415926))
	{
		heading -= (2 * 3.1415926);
	}
	while (heading < (-2 * 3.1415926))
	{
		heading += (2 * 3.1415926);
	}
}

void Entity::RotBy(const float th)
{
	heading += th;
	while (heading > (2 * 3.1415926))
	{
		heading -= (2 * 3.1415926);
	}
	while (heading < (-2 * 3.1415926))
	{
		heading += (2 * 3.1415926);
	}
}

Vec<float> Entity::GetHeading() const
{
	return { cos(heading), sin(heading) };
}



Vec<float> Entity::GetVel() const
{
	return vel;
}

void Entity::SetVel(const Vec<float> newvel)
{
	vel = newvel;
}



Drawable Entity::GetDrawable(bool debug) const
{
	Drawable d(model, c);
	switch (depth)
	{
	case CollDepth::Collided:
		d = Drawable(model, Colors::Red);
		break;
	case CollDepth::MidField:
		d = Drawable(model, Colors::Yellow);
		break;
	case CollDepth::FarField:
		d = Drawable(model, Colors::Green);
		break;
	default:
		break;
		
	}
	depth = CollDepth::Free;
	d.Rot(heading);
	d.Scale(scale);
	d.Translate(pos);
	return d;
}

std::list<Drawable> Entity::GetDrawList() const
{
	std::list<Drawable> drawlist;

	std::vector<Color> c;

	c.emplace_back(Colors::Red);
	c.emplace_back(Colors::Yellow);
	c.emplace_back(Colors::Green);
	c.emplace_back(Colors::LightBlue);
	c.emplace_back(Colors::Blue);
	c.emplace_back(Colors::Magenta);
	c.emplace_back(Colors::DarkGrey);
	c.emplace_back(Colors::White);

	int i = 0;

	for (auto it = modelprimitives.begin(); it != modelprimitives.end(); ++it)
	{
		Drawable d(*it, c[i % 8]);
		d.Rot(heading);
		d.Scale(scale);
		d.Translate(pos);
		++i;

		drawlist.emplace_back(std::move(d));
	}

	return drawlist;
}

void Entity::SetColor(const Color cnew)
{
	c = cnew;
}

//Consider a collision manager
std::vector<int> Entity::CollWith(const Entity& targ) const
{
	return std::vector<int>();
}

// Defunct version
/*
std::vector<int> Entity::CollWith(const Entity& targ) const
{
	std::vector<int> interiorVerts;

	std::vector<Vec<float>>& source = GetTransformedModel();

	float txMin = targ.pos.X - targ.boundingrad;
	float txMax = targ.pos.X + targ.boundingrad;
	float tyMin = targ.pos.Y - targ.boundingrad;
	float tyMax = targ.pos.Y + targ.boundingrad;

	for (int i = 0; i < int(source.size()); ++i)
	{
		Vec<float> s0 = source[i];
		
		if ( s0.X > txMin && s0.X < txMax)
		{
			if (s0.Y > tyMin && s0.Y < tyMax)
			{
				if (targ.CollPoint(s0))
				{
					interiorVerts.emplace_back(i);
				}
			}
		}
	}

	return std::move(interiorVerts);
}
*/


void Entity::Recoil(std::unique_ptr<ActiveEdge>& contactEdge, Entity& targ, float rewindTime)
{
	auto ast = GetTransformedModel();
	auto ship = targ.GetTransformedModel();
	ActiveEdge etTuBrute = *contactEdge;
	LastColl ShipStart = targ.ReadHistory();
	if (contactEdge->n0 == Vec<float>{0.0f, 0.0f})
	{
		bool brokeColl = true;
		if (false)
		{
			ResetHistory();
			targ.ResetHistory();
			return;
		}
	}
	
	TranslateBy(-vel * rewindTime);
	RotBy(-rot * rewindTime);
	targ.TranslateBy(-targ.GetVel() * rewindTime);
	targ.RotBy(-targ.rot * rewindTime);

	/*
	//Temporary Physical Parameter estimation (everything is spherical cows)
	//should be cached later and updated seemlessly for performance
	Vec<float> sourceCoM = {0.0f, 0.0f};
	for each (auto v in GetTransformedModel())
	{
		sourceCoM += v;
	}
	sourceCoM *= (1 / ((float)model.size()));
	Vec<float> targetCoM = { 0.0f, 0.0f };
	for each (auto v in targ.GetTransformedModel())
	{
		targetCoM += v;
	}
	targetCoM *= (1 / ((float)targ.model.size()));
	*/

	//mass prop to volume
	float sourceMass = powf(boundingrad, 3.0f);
	float targMass = powf(targ.boundingrad, 3.0f);
	//moment of Inertia for solid sphere, I =  2/5 * M * R^2 = 2/5 R^5
	float sourceI = 0.4f * powf(boundingrad, 5.0f);
	float targI = 0.4f * powf(targ.boundingrad, 5.0f);


	Vec<float> contact = contactEdge->p0;
	Vec<float> sourceRad = contact - pos;
	Vec<float> targetRad = contact - targ.pos;
	Vec<float> norm = contactEdge->n0.Norm();
	Vec<float> vContactSource = vel + Vec<float>{-sourceRad.Y, sourceRad.X}*rot;
	Vec<float> vContactTarget = targ.vel + Vec<float>{-targetRad.Y, targetRad.X}*rot;

	Vec<float> rvel;
	if (contactEdge->edgeIsA)
	{
		rvel = - vContactTarget + vContactSource;
	}
	else
	{
		rvel = vContactTarget - vContactSource;
	}
	

	if (rvel.Dot(norm) > 0.0f)
	{
		//Never should have come here!
		TranslateBy(vel * rewindTime);
		RotBy(rot * rewindTime);
		targ.TranslateBy(targ.GetVel() * rewindTime);
		targ.RotBy(targ.rot * rewindTime);

		return;
	}
	if (targ.didColl)
	{
		bool STOP = true;
	}

	auto astAft = GetTransformedModel();
	auto shipAft = targ.GetTransformedModel();

	float sourceCross = sourceRad.Cross(norm);
	float targCross = targetRad.Cross(norm);

	float momInertiaFactor = 
		(Vec<float>{ -sourceCross*sourceRad.Y, sourceCross*sourceRad.X }*(1/sourceI)
		+ Vec<float>{ -targCross*targetRad.Y, targCross*targetRad.X }*(1/targI)).Dot(norm);

	//collision response
	float impulse = -(1.f+1.f)* rvel.Dot(norm) / ((1.f/sourceMass) + (1.f/targMass) + momInertiaFactor);
/*
	if ( (impulse > 700.0f) && ( (didColl == true) || (targ.didColl == true) ) )
	{
		bool desired = false;

		if (desired)
		{
			targ.ResetHistory();
			ResetHistory();

			std::vector<int> collider = CollWith(targ);

			if (collider.size() != 0)
			{
				Recoil(collider, targ);
			}
			else
			{
				collider = targ.CollWith(*this);
				targ.Recoil(collider, *this);
			}
		}
	}
*/
	if (contactEdge->edgeIsA)
	{
		vel += norm * impulse / sourceMass;
		targ.vel -= norm * impulse / targMass;
		rot += impulse * sourceRad.Cross(norm) / sourceI;
		targ.rot -= impulse * targetRad.Cross(norm) / targI;
	}
	else
	{
		vel -= norm * impulse / sourceMass;
		targ.vel += norm * impulse / targMass;
		rot -= impulse * sourceRad.Cross(norm) / sourceI;
		targ.rot += impulse * targetRad.Cross(norm) / targI;
	}
	
	//Verification
	sourceRad = contact - pos;
	targetRad = contact - targ.pos;
	vContactSource = vel + Vec<float>{-sourceRad.Y, sourceRad.X}*rot;
	vContactTarget = targ.vel + Vec<float>{-targetRad.Y, targetRad.X}*rot;
	Vec<float> rvel2;
	if (contactEdge->edgeIsA)
	{
		rvel2 = -vContactTarget + vContactSource;
	}
	else
	{
		rvel2 = vContactTarget - vContactSource;
	}

	float radv1 = rvel.Dot(norm);
	float radv2 = rvel2.Dot(norm);
	if (rvel2.Dot(norm) < 0.0f)
	{
		bool stillBroke = true;
	}

	TranslateBy(vel * rewindTime);
	RotBy(rot * rewindTime);
	targ.TranslateBy(targ.GetVel() * rewindTime);
	targ.RotBy(targ.rot * rewindTime);

	auto astFix = GetTransformedModel();
	auto shipFix = targ.GetTransformedModel();

}



bool Entity::CollPoint(const Vec<float> targ) const
{
	std::vector<Vec<float>> sourceTrans = GetTransformedModel();
	Vec<float> refpoint = Vec<float>{ boundingrad + 1.0f, 0.0f } + pos;

	int CollCount = 0;

	for (int i = 0; i < int(model.size()); ++i)
	{
		Vec<float> t0 = sourceTrans[i];
		Vec<float> t1 = sourceTrans[(i + 1) % model.size()];

		if ((ClusterArea(targ, refpoint, t0) > 0.0f && ClusterArea(targ, refpoint, t1) < 0.0f) ||
			(ClusterArea(targ, refpoint, t0) < 0.0f && ClusterArea(targ, refpoint, t1) > 0.0f))
		{
			if ((ClusterArea(t0, t1, targ) > 0.0f && ClusterArea(t0, t1, refpoint) < 0.0f) ||
				(ClusterArea(t0, t1, targ) < 0.0f && ClusterArea(t0, t1, refpoint) > 0.0f))
			{
				++CollCount;
			}
		}
	}

	return (bool(CollCount % 2));
}

float Entity::ClusterArea(Vec<float> A, Vec<float> B, Vec<float> C) const
{
	Vec<float> side0 = { B.X - A.X, B.Y - A.Y };
	Vec<float> side1{ C.X - B.X, C.Y - B.Y };

	return side0.Cross(side1);
}

Vec<float> Entity::GetTransformedVertex(int vert) const
{
	Vec<float> v = model[vert];
	float vxtemp = v.X; float vytemp = v.Y;
	v.X = cos(heading) * vxtemp - sin(heading) * vytemp;
	v.Y = sin(heading) * vxtemp + cos(heading) * vytemp;
	v.X *= scale;
	v.Y *= scale;
	v += pos;

	return v;
}

std::vector<Vec<float>> Entity::GetTransformedModel() const
{
	std::vector<Vec<float>> xmodel;

	for (int i = 0; i < model.size(); ++i)
	{
		xmodel.emplace_back(GetTransformedVertex(i));
	}

 	return xmodel;
}

std::vector<std::vector<Vec<float>>> Entity::GetTransformedPrimitives() const
{
	std::vector<std::vector<Vec<float>>> xprims;

	for (auto it = modelprimitives.begin(); it != modelprimitives.end(); it++)
	{
		std::vector<Vec<float>> xmodel = *it;

		for (int i = 0; i < xmodel.size(); ++i)
		{
			Vec<float> v = xmodel[i];
			float vxtemp = v.X; float vytemp = v.Y;
			v.X = cos(heading) * vxtemp - sin(heading) * vytemp;
			v.Y = sin(heading) * vxtemp + cos(heading) * vytemp;
			v.X *= scale;
			v.Y *= scale;
			v += pos;

			xmodel[i] = v;
		}

		xprims.emplace_back(xmodel);
	}

	return xprims;
}

Vec<float> Entity::UntransformPoint(const Vec<float> pnt)
{
	Vec<float> relpnt = pnt;
	relpnt -= pos;
	relpnt *= 1 / scale;
	float tpx = relpnt.X; float tpy = relpnt.Y;
	relpnt.X = cos(-heading) * tpx - sin(-heading) * tpy;
	relpnt.Y = sin(-heading) * tpx + cos(-heading) * tpy;

	return relpnt;
}



Entity::CollInfo Entity::CalculateImpact(const Vec<float> point, const Vec<float> velocity) const
{
	std::vector<Vec<float>> source = GetTransformedModel();
	Vec<float> rvel = velocity - vel;
	float scaleTime;

	if (abs(rvel.X) > abs(rvel.Y))
	{
		scaleTime = 2 * boundingrad / abs(rvel.X);
	}
	else if(abs(rvel.Y) > abs(rvel.X))
	{
		scaleTime = 2 * boundingrad / abs(rvel.Y);
	}

	Vec<float> refpoint = point + rvel * scaleTime;

	float impactDepth = 2.0 * boundingrad;
	Vec<int> impactSide;

	for (int s0i = 0; s0i<int(source.size()); ++s0i)
	{
		int s1i = (s0i + 1) % int(model.size());
		Vec<float> s0 = source[s0i];
		Vec<float> s1 = source[s1i];

		if ((ClusterArea(point, refpoint, s0) > 0.0f && ClusterArea(point, refpoint, s1) < 0.0f) ||
			(ClusterArea(point, refpoint, s0) < 0.0f && ClusterArea(point, refpoint, s1) > 0.0f))
		{
			if ((ClusterArea(s0, s1, point) > 0.0f && ClusterArea(s0, s1, refpoint) < 0.0f) ||
				(ClusterArea(s0, s1, point) < 0.0f && ClusterArea(s0, s1, refpoint) > 0.0f))
			{
				Vec<float> edge = { s1 - s0 };
				float sideLength = edge.GetLength();
				Vec<float> norm = Vec<float>{ edge.Y, -edge.X }/sideLength;


				float depthFromSide = abs((s0 - point).Cross(s1 - point)) / sideLength;
				float impactVel = rvel.Dot(norm);

				if (depthFromSide < impactDepth)
				{
					impactDepth = depthFromSide;
					impactSide = { s0i, s1i };
				}
			}
		}
	}

	return std::move(CollInfo{ impactDepth, impactSide });
}


void Entity::SetModel(std::vector<Vec<float>> modelnew)
{
	model = std::move(modelnew);
}


std::wstring Entity::ColliderException::GetFullMessage() const
{
	return GetNote();
}
std::wstring Entity::ColliderException::GetExceptionType() const
{
	return L"Breeze Physics Exception";
}

void Entity::SetHistory()
{
	History.pos = pos;
	History.vel = vel;
	History.heading = heading;
	History.rot = rot;
	History.model = GetTransformedModel();
	History.primitives = GetTransformedPrimitives();
}

LastColl Entity::ReadHistory() const
{
	return History;
}

void Entity::ResetHistory()
{
	pos = History.pos;
	vel = History.vel;
	heading = History.heading;
	rot = History.rot;
}

void Entity::ForgeHistory(const LastColl& forgery)
{
	History.pos = forgery.pos;
	History.vel = forgery.vel;
	History.heading = forgery.heading;
	History.rot = forgery.rot;
	History.model = forgery.model;
	History.primitives = forgery.primitives;
}

//Axis-aligned boxes
Rect<float> Entity::GetBoundingBox(const std::vector<Vec<float>>& model) const
{
	float x0 = model[0].X; float x1 = model[0].X;
	float y0 = model[0].Y; float y1 = model[0].Y;

	for(auto v : model)
	{
		if (v.X < x0)
		{
			x0 = v.X;
		}
		else if (v.X > x1)
		{
			x1 = v.X;
		}

		if (v.Y < y0)
		{
			y0 = v.Y;
		}
		else if (v.Y > y1)
		{
			y1 = v.Y;
		}
	}

	return Rect<float>(x0, y0, x1, y1);
}

Rect<float> Entity::GetBoundingBox() const
{
	return GetBoundingBox(GetTransformedModel());
}

