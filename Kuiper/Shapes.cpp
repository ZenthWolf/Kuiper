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
#include <algorithm>
#include "Shapes.h"

//Tolerance 
#define EPS 0.1

std::vector<std::vector<Vec<float>>> Shapes::ConvexSeparator(std::vector<Vec<float>>& model)
{
	std::vector<std::vector<Vec<float>>> primitives;

	int nverts = model.size();
	for (int i = 0; i < nverts; ++i)
	{
		//indecies
		int i0 = i;
		int i1 = (i + 1) % nverts;
		int i2 = (i + 2) % nverts;

		//points indexed
		Vec<float> p0 = model[i0];
		Vec<float> p1 = model[i1];
		Vec<float> p2 = model[i2];

		//Determine p1 relative to line p0-p2
		//This is mathemtically equivalent to Entity::ClusterArea - investigate efficiencies
		float orientation = p0.Cross(p1) + p1.Cross(p2) + p2.Cross(p0);

		if (orientation < 0)
		{
			//Concavity found
			int j0; int h;
			int j1; int k;
			Vec<float> r0;
			Vec<float> r1;

			Vec<float> hit;

			float minLength = FLT_MAX;

			for (int j = 0; j < nverts; ++j)
			{
				if (j != i0 && j != i1)
				{
					j0 = j;
					j1 = (j+1)%nverts;

					r0 = model[j0];
					r1 = model[j1];

					Vec<float> intersection = FindIntersection(p0, p1, r0, r1);

					if( IsOnSegment(p1, p0, intersection) )
					{
						if( IsOnSegment(intersection, r0, r1) )
						{
							float dx = p1.X - intersection.X;
							float dy = p1.Y - intersection.Y;
							float s = dx * dx + dy * dy;

							if (s < minLength)
							{
								h = j0;
								k = j1;
								hit = intersection;
								minLength = s;
							}
						}
					}
				}
			}
			
			//Need to give error if minLength is FLT_MAX

			j0 = h;
			j1 = k;

			//may be convex primitives or not
			std::vector<Vec<float>> precursor0;
			std::vector<Vec<float>> precursor1;

			r0 = model[j0];
			r1 = model[j1];

			//Cut apart into 2 polygons and rerun or find that current polygon is convex
			if(!IsSamePoint(hit, r0))
			{
				precursor0.emplace_back(hit);
			}
			if (!IsSamePoint(hit, r1))
			{
				precursor1.emplace_back(hit);
			}

			h = -1;
			k = i0;
			while (true)
			{

				if (k != j1) precursor0.emplace_back(model[k]);
				else
				{
					//if (h < 0 || h >= n) err();
					if (!IsOnSegment(r1, model[h], p0))
					{
						precursor0.emplace_back(model[k]);
					}
					break;
				}

				h = k;

				//A curse be upon the C++ modulus operator
				if (k - 1 < 0) k = nverts - 1;
				else k--;
			}

			//with thought, this could be avoided
			std::reverse(precursor0.begin(), precursor0.end());

			h = -1;
			k = i1;
			while (true)
			{
				if (k != j1) precursor1.emplace_back(model[k]);
				else
				{
					//if (h < 0 || h >= n) err();
					if (!IsOnSegment(r0, model[h], p1))
					{
						precursor0.emplace_back(model[k]);
					}
					break;
				}

				h = k;
				k = (k + 1) % nverts;
			}


			auto primlist0 = ConvexSeparator(precursor0);
			auto primlist1 = ConvexSeparator(precursor1);
			primitives.insert(primitives.end(), primlist0.begin(), primlist0.end());
			primitives.insert(primitives.end(), primlist1.begin(), primlist1.end());

			return primitives;
		}
	}

	primitives.emplace_back(model);
	return primitives;
}

void Shapes::Center(std::vector<Vec<float>>& shape)
{
	Vec<float> sourceCoM = { 0.0f, 0.0f };
	for each (auto v in shape)
	{
		sourceCoM += v;
	}
	sourceCoM *= (1.0f / ((float)shape.size()));

	for each (auto v in shape)
	{
		v-= sourceCoM;
	}
}

Vec<float> Shapes::FindIntersection(const Vec<float>& p0, const Vec<float>& p1, const Vec<float>& r0, const Vec<float>& r1)
{
	Vec<float> lineP = p1 - p0;
	Vec<float> lineR = r1 - r0;
	Vec<float> lineJoint = r0 - p0;

	float distScale = lineR.Cross(lineJoint) / lineR.Cross(lineP);

	return p0 + lineP * distScale;
}

bool Shapes::IsOnSegment(const Vec<float>& p, const Vec<float>& l0, const Vec<float>& l1)
{
	if ((l0.X + EPS >= p.X && p.X >= l1.X - EPS) || (l0.X - EPS <= p.X && p.X <= l1.X + EPS))
	{
		if ((l0.Y + EPS >= p.Y && p.Y >= l1.Y - EPS) || (l0.Y - EPS <= p.Y && p.Y <= l1.Y + EPS))
		{
			if (abs(l0.X - l1.X) > EPS)
			{
				float slope = (l1.Y - l0.Y) / (l1.X - l0.X);
				float testY = slope * (p.X - l0.X) + l0.Y;

				float diff = abs(testY - p.Y);
				return (diff < EPS);
			}

			return (abs(p.X - l0.X) < EPS);
		}
	}
	return false;
}

bool Shapes::IsSamePoint(const Vec<float>& p, const Vec<float>& r)
{
	float dx = abs(p.X - r.X);
	float dy = abs(p.Y - r.Y);
	return (dx < EPS && dy < EPS);
}
