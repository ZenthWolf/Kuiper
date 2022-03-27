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

#include "ColliderManager.h"

Entity::Entity(std::vector<Vec<float>> modl, const Vec<float>& pos = { 0.0f, 0.0f }, 
			const Vec<float>& vel = { 0.0f, 0.0f }, float angvel = 0.0f, Color c = Colors::White)
	:model(std::move(modl)), pos(pos), vel(vel), angvel(angvel), c(c)
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

	cachedModel = model;
	cachedPrimitives = modelprimitives;
	SetHistory();
}


//Parameter: Get/Set/Update
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

Vec<float> Entity::GetVel() const
{
	return vel;
}

void Entity::SetVel(const Vec<float> newvel)
{
	vel = newvel;
}

Vec<float> Entity::GetHeading() const
{
	return { cos(heading), sin(heading) };
}

void Entity::SetHeading(const float th)
{
	heading = th;
	while (heading > (6.283185))
	{
		heading -= (6.283185);
	}
	while (heading < (-6.283185))
	{
		heading += (6.283185);
	}
}

void Entity::RotBy(const float th)
{
	heading += th;
	while (heading > (6.283185))
	{
		heading -= (6.283185);
	}
	while (heading < (-6.283185))
	{
		heading += (6.283185);
	}
}

float Entity::GetAngVel() const
{
	return angvel;
}

void Entity::SetAngVel(const float newvel)
{
	angvel = newvel;
}

float Entity::GetScale() const
{
	return scale;
}

void Entity::SetScale(const float s)
{
	scale = s;
	boundingrad *= s;
}

float Entity::GetRadius() const
{
	return boundingrad;
}

//Dynamic Qualities
//Axis-aligned boxes
Rect<float> Entity::GetBoundingBox() const
{
	return Shapes::BoundingBox(GetTransformedModel());
}

std::vector<Vec<float>> Entity::GetTransformedModel() const
{
	if (staleModel)
	{
		for (int i = 0; i < cachedModel.size(); ++i)
		{
			cachedModel[i] = GetTransformedVertex(i);
		}
		staleModel = false;
	}
	return cachedModel;
}

std::vector<std::vector<Vec<float>>> Entity::GetTransformedPrimitives() const
{
	if (stalePrimitives)
	{
		for (int i = 0; i < cachedPrimitives.size(); ++i)
		{
			for (int j = 0; j < cachedPrimitives[i].size(); ++j)
			{
				Vec<float> v = modelprimitives[i][j];
				float vxtemp = v.X; float vytemp = v.Y;
				v.X = cos(heading) * vxtemp - sin(heading) * vytemp;
				v.Y = sin(heading) * vxtemp + cos(heading) * vytemp;
				v.X *= scale;
				v.Y *= scale;
				v += pos;

				cachedPrimitives[i][j] = v;
			}
		}
		stalePrimitives = false;
	}

	return cachedPrimitives;
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

void Entity::alertStaleModel() const
{
	staleModel = true;
	stalePrimitives = true;
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

//Rendering
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

//Interactions
void Entity::Recoil(const ActiveEdge& contactEdge, Entity& targ, const float rewindTime, int& jank, bool doJank)
{
	if (doJank && !contactEdge.discreteCollision)
	{
		jank++;
	}
	alertStaleModel();
	targ.alertStaleModel();

	TranslateBy(-vel * rewindTime);
	RotBy(-angvel * rewindTime);
	targ.TranslateBy(-targ.GetVel() * rewindTime);
	targ.RotBy(-targ.angvel * rewindTime);

	//mass prop to volume
	float sourceMass = powf(boundingrad, 3.0f);
	float targMass = powf(targ.boundingrad, 3.0f);
	//moment of Inertia for solid sphere, I =  2/5 * M * R^2 = 2/5 R^5
	float sourceI = 0.4f * powf(boundingrad, 5.0f);
	float targI = 0.4f * powf(targ.boundingrad, 5.0f);

	Vec<float> norm = contactEdge.n0.Norm();

	if (contactEdge.discreteCollision)
	{
		if (contactEdge.edgeIsA)
		{
			TranslateBy(norm * contactEdge.depth1 * 0.5f);
			targ.TranslateBy(-norm * contactEdge.depth1 * 0.5f);
		}
		else
		{
			TranslateBy(-norm * contactEdge.depth1 * 0.5f);
			targ.TranslateBy(norm * contactEdge.depth1 * 0.5f);
		}
	}

	Vec<float> contact = contactEdge.p0;
	Vec<float> sourceRad = contact - pos;
	Vec<float> targetRad = contact - targ.pos;
	
	Vec<float> vContactSource = vel + Vec<float>{-sourceRad.Y, sourceRad.X}*angvel;
	Vec<float> vContactTarget = targ.vel + Vec<float>{-targetRad.Y, targetRad.X}*angvel;

	Vec<float> rvel;
	if (contactEdge.edgeIsA)
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
		RotBy(angvel * rewindTime);
		targ.TranslateBy(targ.GetVel() * rewindTime);
		targ.RotBy(targ.angvel * rewindTime);

		return;
	}
	if (doJank && contactEdge.discreteCollision)
	{
		jank++;
	}

	float sourceCross = sourceRad.Cross(norm);
	float targCross = targetRad.Cross(norm);

	float momInertiaFactor = 
		(Vec<float>{ -sourceCross*sourceRad.Y, sourceCross*sourceRad.X }*(1/sourceI)
		+ Vec<float>{ -targCross*targetRad.Y, targCross*targetRad.X }*(1/targI)).Dot(norm);

	//collision response
	float impulse = -(1.f+1.f)* rvel.Dot(norm) / ((1.f/sourceMass) + (1.f/targMass) + momInertiaFactor);

	if (contactEdge.edgeIsA)
	{
		vel += norm * impulse / sourceMass;
		targ.vel -= norm * impulse / targMass;
		angvel += impulse * sourceRad.Cross(norm) / sourceI;
		targ.angvel -= impulse * targetRad.Cross(norm) / targI;
	}
	else
	{
		vel -= norm * impulse / sourceMass;
		targ.vel += norm * impulse / targMass;
		angvel -= impulse * sourceRad.Cross(norm) / sourceI;
		targ.angvel += impulse * targetRad.Cross(norm) / targI;
	}
	
	//Verification
	sourceRad = contact - pos;
	targetRad = contact - targ.pos;
	vContactSource = vel + Vec<float>{-sourceRad.Y, sourceRad.X}*angvel;
	vContactTarget = targ.vel + Vec<float>{-targetRad.Y, targetRad.X}*angvel;
	Vec<float> rvel2;
	if (contactEdge.edgeIsA)
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
	RotBy(angvel * rewindTime);
	targ.TranslateBy(targ.GetVel() * rewindTime);
	targ.RotBy(targ.angvel * rewindTime);
}

/// <summary>
/// Determines if a point is interior to the model of the Entity
/// Draws a ray to the exterior, and counts number of collisions to do so.
/// </summary>
/// <param name="targ">Point to be queried</param>
/// <returns>true for targ interior to entity model</returns>
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

/// <summary>
/// Returns cross product of triangle bound by points given.
/// Usage for determining relative orientation of points.
/// </summary>
/// <returns>positive if A-B-C is a counterclockwise orientation</returns>
float Entity::ClusterArea(Vec<float> A, Vec<float> B, Vec<float> C) const
{
	//Equiv to (B-A)X(C-B)
	return A.Cross(B) + B.Cross(C) + C.Cross(A);
}

//History Recording
void Entity::SetHistory()
{
	History.pos = pos;
	History.vel = vel;
	History.heading = heading;
	History.angvel = angvel;
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
	angvel = History.angvel;
}
