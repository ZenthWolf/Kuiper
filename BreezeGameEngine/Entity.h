#pragma once

#include <vector>
#include "Vec.h"
#include "Colors.h"
#include "Drawable.h"

class Entity
{
public:
	Entity(std::vector<Vec<float>> modl, const Vec<float>& pos = { 0.0f, 0.0f }, const Vec<float>& vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::White)
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
	virtual ~Entity() = default;

	const Vec<float>& GetPos() const
	{
		return pos;
	}

	void SetPos(const Vec<float> newpos)
	{
		pos = newpos;
	}

	void TranslateBy(const Vec<float> dpos)
	{
		pos += dpos;
	}

	void SetScale(const float s)
	{
		scale = s;
		boundingrad *= s;
	}

	float GetScale() const
	{
		return scale;
	}

	float GetRadius() const
	{
		return boundingrad;
	}

	void SetHeading(const float th)
	{
		heading = th;
		while (heading > (2 * 3.1415926))
		{
			heading -= (2 * 3.1415926);
		}
	}

	void RotBy(const float th)
	{
		heading += th;
		while (heading > (2 * 3.1415926))
		{
			heading -= (2 * 3.1415926);
		}
	}

	Vec<float> GetHeading() const
	{
		return {cos(heading), sin(heading)};
	}

	Drawable GetDrawable() const
	{
		Drawable d(model, c);
		d.Rot(heading);
		d.Scale(scale);
		d.Translate(pos);
		return d;
	}

	void SetColor(Color cnew)
	{
		c = cnew;
	}

	bool CollWith(Entity targ)
	{
		std::vector<Vec<float>>& source = GetTransformedModel();
		std::vector<Vec<float>>& target = targ.GetTransformedModel();

		for (int i = 0; i < int(source.size()); i++)
		{
			Vec<float> s0 = source[i];
			Vec<float> s1 = source[(i+1) % source.size()];

			for (int j = 0; j < int(target.size()); j++)
			{
				Vec<float> t0 = target[j];
				Vec<float> t1 = target[(j+1) % source.size()];

				if ((ClusterArea(s0, s1, t0) > 0.0f && ClusterArea(s0, s1, t1) < 0.0f) ||
					(ClusterArea(s0, s1, t0) < 0.0f && ClusterArea(s0, s1, t1) > 0.0f))
				{
					if ((ClusterArea(t0, t1, s0) > 0.0f && ClusterArea(t0, t1, s1) < 0.0f) ||
						(ClusterArea(t0, t1, s0) < 0.0f && ClusterArea(t0, t1, s1) > 0.0f))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	std::vector<Vec<float>> GetTransformedModel() const
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

	void Recoil(Entity& targ)
	{
		Vec<float> impact = (targ.pos - pos).Norm();

		if (impact.Dot(targ.vel - vel) < 0.0f)
		{
			float angimpact = impact.Cross(targ.vel + vel);
			float aimp0 = angimpact / (targ.boundingrad* targ.boundingrad);
			float aimp1 = angimpact / (boundingrad*boundingrad);

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
		}
	}

protected:
	void SetModel(std::vector<Vec<float>> modelnew)
	{
		model = std::move(modelnew);
	}

	float ClusterArea(Vec<float> A, Vec<float> B, Vec<float> C)
	{
		Vec<float> side0 = { B.X - A.X, B.Y - A.Y};
		Vec<float> side1 { C.X - B.X, C.Y - B.Y };

		return side0.Cross(side1);
	}

	std::vector<Vec<float>> model;

	Color c;

	Vec<float> pos = { 0.0f, 0.0f };
	Vec<float> vel = { 0.0f, 0.0f };
	float vmax = 200.0f;

	float heading = 0.0f;
	float rot = 0.0f;
	float rotmax = 3.0f;

	float scale = 1.0f;
	float boundingrad = 0.0f;
};