#include "Entity.h"
#include<algorithm>
#include<iostream>

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


Entity::Intersection Entity::CollWith(const Entity& targ)
{
	std::vector<Vec<float>>& source = GetTransformedModel();
	std::vector<Vec<float>>& target = targ.GetTransformedModel();

	std::vector<Vec<int>> sourceLinesIndex;
	std::vector<Vec<int>> targetLinesIndex;

	for (int i = 0; i < int(source.size()); i++)
	{
		Vec<float> s0 = source[i];
		Vec<float> s1 = source[(i + 1) % int(source.size())];

		for (int j = 0; j < int(target.size()); j++)
		{
			Vec<float> t0 = target[j];
			Vec<float> t1 = target[(j + 1) % target.size()];

			if ((ClusterArea(s0, s1, t0) > 0.0f && ClusterArea(s0, s1, t1) < 0.0f) ||
				(ClusterArea(s0, s1, t0) < 0.0f && ClusterArea(s0, s1, t1) > 0.0f))
			{
				if ((ClusterArea(t0, t1, s0) > 0.0f && ClusterArea(t0, t1, s1) < 0.0f) ||
					(ClusterArea(t0, t1, s0) < 0.0f && ClusterArea(t0, t1, s1) > 0.0f))
				{
					sourceLinesIndex.emplace_back(Vec<int>{ i, (i + 1) % int(source.size()) });

					targetLinesIndex.emplace_back(Vec<int>{ j, (j + 1) % int(target.size()) });
				}
			}
		}
	}

	return Intersection{ sourceLinesIndex,targetLinesIndex, source, target };
}

void Entity::Recoil(Entity::Intersection intersection, Entity& targ)
{
	assert(intersection.passed < 2);
	intersection.passed++;

	std::vector<Entity::vertexType> pntloc = ResolveInternalVertex(intersection, targ);

	if (pntloc.size() == 0)
	{
		if (intersection.passed < 2)
		{
			bool scream = true;
			std::swap(intersection.sourceLinesIndex, intersection.targetLinesIndex);
			std::swap(intersection.sourceTrans, intersection.targetTrans);
			targ.Recoil(intersection, *this);
		}
		else
		{
			throw BREEZE_COLLIDER_EXCEPTION(L"Collider failed to resolve interior vertecies");
		}
	}

	else
	{
		int counter = 0;
		for (auto v : pntloc)
		{
			if (v == Entity::vertexType::inside)
			{
				counter++;
			}
		}
		std::cout << counter;
		std::vector<Vec<int>> sourceLinesIndex = intersection.sourceLinesIndex;
		std::vector<Vec<int>> targetLinesIndex = intersection.targetLinesIndex;
		std::vector<Vec<float>> sourceTrans = intersection.sourceTrans;
		std::vector<Vec<float>> targetTrans = intersection.targetTrans;


		Vec<float> debugshift = { 0.0f, 0.0f };

		for (int i = 0; i<int(sourceLinesIndex.size()); i++)
		{
			int s0i = sourceLinesIndex[i].X;
			int s1i = sourceLinesIndex[i].Y;

			int t0i = targetLinesIndex[i].X;
			int t1i = targetLinesIndex[i].Y;

			if (pntloc[s0i] == Entity::vertexType::inside)
			{
				Vec<float> s0 = sourceTrans[s0i];
				Vec<float> t0 = targetTrans[t0i];
				Vec<float> t1 = targetTrans[t1i];
				pntloc[s0i] = Entity::vertexType::outside;

				debugshift += MomentumTransfer(s0, t0, t1, targ);
			}
		}

		for (int i = 0; i<int(model.size()); i++)
		{
			if (targ.CollPoint(model[i]))
			{
				bool scream = true;
				pos -= debugshift / 2;
				targ.pos += debugshift / 2;
				intersection.passed = 0;
				//Recoil(intersection, targ);
			}
		}
	}
}

std::vector<Entity::vertexType> Entity::ResolveInternalVertex(Entity::Intersection intersection, Entity& targ)
{

	std::vector<Vec<int>> sourceLinesIndex = intersection.sourceLinesIndex;
	std::vector<Vec<int>> targetLinesIndex = intersection.targetLinesIndex;
	std::vector<Vec<float>> sourceTrans = intersection.sourceTrans;


	std::vector<Entity::vertexType> pntloc;

	for (int i = 0; i<int(model.size()); i++)
	{
		pntloc.emplace_back(Entity::vertexType::unchecked);
	}

	bool pntFound = false;

	for (int i = 0; i<int(sourceLinesIndex.size()); i++)
	{
		int s0 = sourceLinesIndex[i].X;
		int s1 = sourceLinesIndex[i].Y;

		if (pntloc[s0] == vertexType::unchecked)
		{
			Vec<float> vertex0 = sourceTrans[s0];
			if (targ.CollPoint(vertex0))
			{
				pntloc[s0] = vertexType::inside;
				pntFound = true;
			}
			else
			{
				pntloc[s0] = vertexType::outside;
			}
		}

		if (pntloc[s1] == vertexType::unchecked)
		{
			Vec<float> vertex1 = sourceTrans[s1];
			if (targ.CollPoint(vertex1))
			{
				pntloc[s1] = vertexType::inside;
				pntFound = true;
			}
			else
			{
				pntloc[s1] = vertexType::outside;
			}
		}
	}

	if (!pntFound)
	{
		return std::vector<Entity::vertexType>();
	}

	else
	{
		return pntloc;
	}
}

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
		/*Olde*/
		Vec<float> impact = (targ.pos - pos).Norm();

		if (impact.Dot(targ.vel - vel) < 0.0f)
		{
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



bool Entity::CollPoint(const Vec<float> targ)
{
	Vec<float> target = UntransformPoint(targ);
	Vec<float> refpoint = { boundingrad/scale + 1.0f, 0.0f };

	int CollCount = 0;

	for (int i = 0; i < int(model.size()); i++)
	{
		Vec<float> t0 = model[i];
		Vec<float> t1 = model[(i + 1) % model.size()];

		if ((ClusterArea(target, refpoint, t0) > 0.0f && ClusterArea(target, refpoint, t1) < 0.0f) ||
			(ClusterArea(target, refpoint, t0) < 0.0f && ClusterArea(target, refpoint, t1) > 0.0f))
		{
			if ((ClusterArea(t0, t1, target) > 0.0f && ClusterArea(t0, t1, refpoint) < 0.0f) ||
				(ClusterArea(t0, t1, target) < 0.0f && ClusterArea(t0, t1, refpoint) > 0.0f))
			{
				CollCount++;
			}
		}
	}

	return (bool(CollCount % 2));
}

float Entity::ClusterArea(Vec<float> A, Vec<float> B, Vec<float> C)
{
	Vec<float> side0 = { B.X - A.X, B.Y - A.Y };
	Vec<float> side1{ C.X - B.X, C.Y - B.Y };

	return side0.Cross(side1);
}


std::vector<Vec<float>> Entity::GetTransformedModel() const
{
	const auto xform = [&](Vec<float> v)
	{
		float vxtemp = v.X; float vytemp = v.Y;
		v.X = cos(heading) * vxtemp - sin(heading) * vytemp;
		v.Y = sin(heading) * vxtemp + cos(heading) * vytemp;
		v.X *= scale;
		v.Y *= scale;
		v += pos;

		return v;
	};

	std::vector<Vec<float>> xmodel;

	for (int i = 0; i < model.size(); i++)
	{
		xmodel.emplace_back(xform(model[i]));
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