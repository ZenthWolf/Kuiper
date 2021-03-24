#define NOMINMAX // I just want std::min to work, Windows...

#include "Entity.h"
#include<algorithm>

#define BREEZE_COLLIDER_EXCEPTION( note ) Entity::ColliderException( note,_CRT_WIDE(__FILE__),__LINE__ )

Entity::Entity(std::vector<Vec<float>> modl, const Vec<float>& pos = { 0.0f, 0.0f }, const Vec<float>& vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::White)
	:model(std::move(modl)), pos(pos), vel(vel), rot(rot), c(c)
{
	for (auto v : model)
	{
		float vrad = v.GetLengthSq();
		if (vrad > boundingrad)
		{
			boundingrad = vrad;
		}
	}

	boundingrad = sqrtf(boundingrad);
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
}

void Entity::RotBy(const float th)
{
	heading += th;
	while (heading > (2 * 3.1415926))
	{
		heading -= (2 * 3.1415926);
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



Drawable Entity::GetDrawable() const
{
	Drawable d(model, c);
	d.Rot(heading);
	d.Scale(scale);
	d.Translate(pos);
	return d;
}

void Entity::SetColor(Color cnew)
{
	c = cnew;
}


std::vector<int> Entity::CollWith(const Entity& targ) const
{
	std::vector<int> interiorVerts;

	std::vector<Vec<float>>& source = GetTransformedModel();

	float txMin = targ.pos.X - targ.boundingrad;
	float txMax = targ.pos.X + targ.boundingrad;
	float tyMin = targ.pos.Y - targ.boundingrad;
	float tyMax = targ.pos.Y + targ.boundingrad;

	for (int i = 0; i < int(source.size()); i++)
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



void Entity::Recoil(std::vector<int> collider, Entity& targ)
{
	float impactTime = 2 * targ.boundingrad;
	Vec<int> contactSide;
	int contactPoint;

	/**************************************************/
	/*****COLLINFO RETURNS DEPTH NOT TIME**************/
	/**************************************************/


	for (auto vertInd : collider)
	{
		Vec<float> vert = GetTransformedVertex(vertInd);
		Vec<float> vertVel = vel + Vec<float>{-vert.Y, vert.X} *rot;
		CollInfo collision = targ.CalculateImpact(vert, -vel);

		if (impactTime > collision.impactTime)
		{
			impactTime = collision.impactTime;
			contactSide = collision.impactSide;
			contactPoint = vertInd;
		}
	}

	Vec<float> t0 = targ.GetTransformedVertex(contactSide.X);
	Vec<float> t1 = targ.GetTransformedVertex(contactSide.Y);
	Vec<float> contact = GetTransformedVertex(contactPoint);
	Vec<float> sourceRad = contact - pos;
	Vec<float> targetRad = contact - targ.pos;
	Vec<float> edge = { t1 - t0 };
	Vec<float> norm = Vec<float>{ -edge.Y, edge.X }.Norm();
	Vec<float> vContactSource = vel + Vec<float>{-sourceRad.Y, sourceRad.X}*rot;
	Vec<float> vContactTarget = targ.vel + Vec<float>{-targetRad.Y, targetRad.X}*rot;
	Vec<float> rvel = vContactTarget - vContactSource;

	if(rvel.Dot(norm) < 0.0f)
	{
		TranslateBy(-norm * impactTime / 2);
		targ.TranslateBy(norm * impactTime / 2);

		float momInertiaFactor = Vec<float>{ sourceRad.Y, -sourceRad.X }.Dot(norm) / sourceRad.GetLengthSq()
			+ Vec<float>{targetRad.Y, -targetRad.X }.Dot(norm) / targetRad.GetLengthSq();


		float impulse = -2 * rvel.Dot(norm) / (1 + 1 + momInertiaFactor);


		vel -= norm * impulse;
		targ.vel += norm * impulse;
		rot -= impulse * sourceRad.Cross(norm) / sourceRad.GetLengthSq();
		targ.rot += impulse * targetRad.Cross(norm) / targetRad.GetLengthSq();
	}
	/*
	//	assert(intersection.passed < 2);
	intersection.passed++;

	if (intersection.passed > 2)
	{
		bool scream = true;
	}

	if (!(targ.CollPoint(intersection.sourceTrans[intersection.sourceLinesIndex[0].X]) ||
		targ.CollPoint(intersection.sourceTrans[intersection.sourceLinesIndex[0].Y])))
	{
		std::swap(intersection.sourceLinesIndex, intersection.targetLinesIndex);
		std::swap(intersection.sourceTrans, intersection.targetTrans);
		targ.Recoil(intersection, *this);

		if (intersection.passed >= 2)
		{
			throw BREEZE_COLLIDER_EXCEPTION(L"Collider failed to resolve interior vertecies");
		}
	}

	else
	{
		std::vector<int> intVertIndex = ResolveInternalVertex(intersection, targ);

		std::vector<Vec<int>> sourceLinesIndex = intersection.sourceLinesIndex;
		std::vector<Vec<int>> targetLinesIndex = intersection.targetLinesIndex;
		std::vector<Vec<float>> sourceTrans = intersection.sourceTrans;
		std::vector<Vec<float>> targetTrans = intersection.targetTrans;

		int contactPoint = -1;
		Vec<int> contactSide;
		float impactTime = 0.0f;
		float impactDepth;

		//find the longest time for which vertex to leave
		for (auto vertIndex : intVertIndex)
		{
			Vec<float> s0 = sourceTrans[vertIndex];
			float dtVert = -1.0f;
			float depthVert;
			Vec<int> sideVert;

			//find the shortest time for this vert to leave
			for (int i = 0; i<int(targetTrans.size()); i++)
			{
				int t0i = i; int t1i = (i + 1) % int(targetTrans.size());
				Vec<float> t0 = targetTrans[t0i];
				Vec<float> t1 = targetTrans[t1i];

				Vec<float> edge = { t1 - t0 };
				Vec<float> norm = Vec<float>{ edge.Y, -edge.X }.Norm();

				Vec<float> rvel = (targ.vel - vel);

				float depthFromSide = abs((t0 - s0).Cross(t1 - s0)) / edge.GetLength();
				float impactVel = rvel.Dot(norm);
				float dt = depthFromSide / impactVel;

				if (impactVel > 0.0f)
				{
					bool Krezk = true;
				}

				if ( (dt > 0) && ((dt < dtVert) || (dtVert == -1.0f)) )
				{
					dtVert = dt;
					depthVert = depthFromSide;
					sideVert = { t0i, t1i };
				}
			}

			if (dtVert > impactTime)
			{
				contactPoint = vertIndex;
				contactSide = sideVert;
				impactTime = dtVert;
				impactDepth = depthVert;
			}
		}

		if (contactPoint == -1)
		{
			bool beSad = true;
		}

		TranslateBy(vel * impactTime);
		targ.TranslateBy(-targ.vel * impactTime);

		Vec<float> t0 = targetTrans[contactSide.X];
		Vec<float> t1 = targetTrans[contactSide.Y];
		Vec<float> contact = sourceTrans[contactPoint];
		Vec<float> sourceRad = contact - pos;
		Vec<float> targetRad = contact - targ.pos;

		Vec<float> contactVel = vel;

		Vec<float> edge = { t1 - t0 };
		Vec<float> norm = Vec<float>{ edge.Y, -edge.X }.Norm();
		*/
		/*
		for (auto vertIndex : intVertIndex)
		{
			Vec<float> s0 = sourceTrans[vertIndex];
			float bestImpact = 0.0f;

			for (int i = 0; i<int(targetTrans.size()); i++)
			{
				Vec<int> side = { i, (i + 1) % int(targetTrans.size()) };
				Vec<float> t0 = targetTrans[side.X];
				Vec<float> t1 = targetTrans[side.Y];

				Vec<float> edge = { t1 - t0 };
				Vec<float> norm = Vec<float>{ edge.Y, -edge.X }.Norm();

				Vec<float> rvel = (targ.vel - vel);

				float depth = abs((t0 - s0).Cross(t1 - s0)) / edge.GetLength();
				float impact = rvel.Dot(norm);

				if (impact > 0.0f)
				{
					float dt = depth / rvel.GetLength();

					if (dt < impactTime)
					{
						bestImpact = impact;
						impactDepth = depth;
						contactPoint = vertIndex;
						contactSide = side;
					}
				}
			}
		}

		if (contactPoint == -1)
		{
			bool scream = true;
		}

		
		TranslateBy(-vel * dt);
		targ.TranslateBy(targ.vel * dt);

		Vec<float> t0 = targetTrans[contactSide.X];
		Vec<float> t1 = targetTrans[contactSide.Y];
		Vec<float> contact = sourceTrans[contactPoint];
		Vec<float> sourceRad = contact - pos;
		Vec<float> targetRad = contact - targ.pos;

		Vec<float> contactVel = vel;

		Vec<float> edge = { t1 - t0 };
		Vec<float> norm = Vec<float>{ edge.Y, -edge.X }.Norm();

		if (!norm.Dot(contactVel))
		{
			float momInertiaFactor = Vec<float>{ -sourceRad.Y, sourceRad.X }.Dot(norm) / sourceRad.GetLengthSq()
				+ Vec<float>{-targetRad.Y, contact.X }.Dot(norm) / (targetRad - pos).GetLengthSq();

			float impulse = -rvel.Dot(norm) / (1 + 1 + momInertiaFactor);

			vel += norm * impulse;
			targ.vel -= norm * impulse;
			rot -= impulse * sourceRad.Cross(norm);
			targ.rot += impulse * targetRad.Cross(norm);
			Intersection test = CollWith(targ);
			if (test.sourceLinesIndex.size() > 0)
			{
				bool screm = false;
			}
		}
	}
	*/
}


//??Defunct???
Vec<float> Entity::MomentumTransfer(Vec<float> s0, Vec<float> t0, Vec<float>t1, Entity& targ)
{
	Vec<float> edge = { t1 - t0 };
	Vec<float> tang = { edge.Y, -edge.X };
	if (tang.Dot(s0 - t0) > 0.0f)
	{
		tang *= -1;
	}

	Vec<float> vs0 = vel - Vec<float>{s0.Y, -s0.X} *rot;
	if (vs0.Dot(tang) < 0.0f)
	{
		Vec<float> impact = (targ.pos - pos).Norm();
		Vec<float> fulcrum = (targ.pos - s0);

		float angimp = fulcrum.Cross(targ.vel - vel) / (boundingrad * boundingrad);
		targ.rot += angimp / (targ.boundingrad * targ.boundingrad);
		rot -= angimp / (boundingrad * boundingrad);

		Vec<float> xfer = impact * abs((targ.vel - vel).Dot(impact));

		targ.vel += xfer;
		vel -= xfer;

		/*Olde*
		Vec<float> impact = (targ.pos - pos).Norm();


		float angimpact = impact.Cross(targ.vel + vel);
		float aimp0 = angimpact / (targ.boundingrad * targ.boundingrad);
		float aimp1 = angimpact / (boundingrad * boundingrad);

		if (rot > 0.0f && targ.rot > 0.0f)
		{
			targ.rot -= aimp0;
			rot -= aimp1;
		}
		else if (rot < 0.0f && targ.rot < 0.0f)
		{
			targ.rot += aimp0;
			rot += aimp1;
		}
		else if (rot < 0.0f && targ.rot > 0.0f)
		{
			targ.rot -= aimp0;
			rot += aimp1;
		}
		else
		{
			targ.rot += aimp0;
			rot -= aimp1;
		}

		Vec<float> xfer = impact * abs((targ.vel - vel).Dot(impact));

		targ.vel += xfer;
		vel -= xfer;
		/*OLDE*/
	}
	//2*A
	float scaledimpact = abs((t0 - s0).Cross(t1 - s0));

	Vec<float> shift = tang * scaledimpact;
	shift *= 1 / tang.GetLengthSq();

	pos += shift / 2;
	targ.pos -= shift / 2;
	Vec<float> debugshift = shift;

	return debugshift;
}



bool Entity::CollPoint(const Vec<float> targ) const
{
	std::vector<Vec<float>> sourceTrans = GetTransformedModel();
	Vec<float> refpoint = Vec<float>{ boundingrad + 1.0f, 0.0f } + pos;

	int CollCount = 0;

	for (int i = 0; i < int(model.size()); i++)
	{
		Vec<float> t0 = sourceTrans[i];
		Vec<float> t1 = sourceTrans[(i + 1) % model.size()];

		if ((ClusterArea(targ, refpoint, t0) > 0.0f && ClusterArea(targ, refpoint, t1) < 0.0f) ||
			(ClusterArea(targ, refpoint, t0) < 0.0f && ClusterArea(targ, refpoint, t1) > 0.0f))
		{
			if ((ClusterArea(t0, t1, targ) > 0.0f && ClusterArea(t0, t1, refpoint) < 0.0f) ||
				(ClusterArea(t0, t1, targ) < 0.0f && ClusterArea(t0, t1, refpoint) > 0.0f))
			{
				CollCount++;
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

	for (int i = 0; i < model.size(); i++)
	{
		xmodel.emplace_back(GetTransformedVertex(i));
	}

	return xmodel;
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

	//float impactTime = 0.0f;
	float impactTime = 200.0 * boundingrad;
	Vec<int> impactSide;

	for (int s0i = 0; s0i<int(source.size()); s0i++)
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
				float dt = depthFromSide / impactVel;

				/*
				if ( dt > impactTime )
				{
					impactTime = dt;
					impactSide = { s0i, s1i };
				}
				*/
				if (depthFromSide < impactTime)
				{
					impactTime = depthFromSide;
					impactSide = { s0i, s1i };
				}
			}
		}
	}

	return std::move(CollInfo{ impactTime, impactSide });
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