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

std::vector<std::vector<Vec<float>>> Shapes::ConvexSeparator(const std::vector<Vec<float>>& model)
{
	std::vector<std::vector<Vec<float>>> primitives;

	const int nverts = model.size();
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

			//Find first edge intersected by extension
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

			//Indecies of opposing edge
			j0 = h;
			j1 = k;

			//may be convex primitives or not
			std::vector<Vec<float>> precursor0;
			std::vector<Vec<float>> precursor1;

			r0 = model[j0];
			r1 = model[j1];

			//Cut apart into 2 polygons and rerun or find that current polygon is convex
			int precursor0Start = j1;
			int precursor0End = i1;
			int precursor1Start = i1;
			int precursor1End = j1;

			if(IsSamePoint(hit, r0))
			{
				hit = r0;
				precursor1End = j0;
			}
			else if (IsSamePoint(hit, r1))
			{
				hit = r1;
				precursor0Start = (precursor0Start + 1) % nverts;
			}
			precursor0.reserve((i0 - precursor0Start + nverts) % nverts + 1);
			precursor0.emplace_back(hit);
			
			for (k = precursor0Start; k!=precursor0End; k=(k+1)%nverts)
				precursor0.emplace_back(model[k]);

			precursor1.reserve((precursor1End - i1 + nverts) % nverts + 1);
			precursor1.emplace_back(hit);
			for (k = precursor1Start; k!=precursor1End; k=(k+1)%nverts)
				precursor1.emplace_back(model[k]);

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
	sourceCoM *= (1 / ((float)shape.size()));

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

/// <summary>
/// Calculates nearest elements between concave polygons.
/// </summary>
/// <param name="modelList">World Space model</param>
/// <returns>Nearest Elements Information</returns>
Approach Shapes::FindApproach(const std::vector<std::vector<Vec<float>>>& modelList0, const std::vector<std::vector<Vec<float>>>& modelList1)
{
	//All cases for first element in model0
	Approach result = FindApproach(modelList0[0], modelList1[0]);

	for (int j=1; j<modelList1.size(); ++j)
	{
		Approach test = FindApproach(modelList0[0], modelList1[j]);

		if (test.distance < result.distance)
		{
			test.convex0 = 0;
			test.convex1 = j;
			result = std::move(test);
			if (result.distance == 0.0f)
			{
				return result;;
			}
		}
	}

	//Remaining cases
	for (int i=1; i<modelList0.size(); ++i)
	{
		for (int j=0; j<modelList1.size(); ++j)
		{
			Approach test = FindApproach(modelList0[i], modelList1[j]);

			if (test.distance < result.distance)
			{
				test.convex0 = i;
				test.convex1 = j;
				result = std::move(test);
				if (result.distance == 0.0f)
				{
					return result;
				}
			}
		}
	}
	return result;
}

/// <summary>
/// Calculates nearest elements between convex polygons, or convex primitives. 
/// </summary>
/// <param name="model">World Space model</param>
/// <returns>Nearest Elements Information</returns>
Approach Shapes::FindApproach(const std::vector<Vec<float>>& model0,
	const std::vector<Vec<float>>& model1)
{
// Initial Simplex
	Simplex simplex;
	simplex.vertex0.index0 = 0;
	simplex.vertex0.index1 = 0;
	simplex.vertex0.point0 = model0[0];
	simplex.vertex0.point1 = model1[0];
	simplex.vertex0.point = simplex.vertex0.point1 - simplex.vertex0.point0;
	simplex.vertex0.u = 1.0f;
	simplex.count = 1;

	// Pointer to vertecies in simplex, acts like an array
	// Just don't be reckless and go out of bounds
	SimplexVertex* vertices = &simplex.vertex0;

	// Vertices of last simplex from previous iteration
	// Finding a repeat is primary termination criterion.
	int save0[3] = { -1,-1,-1 }, save1[3] = { -1,-1,-1 };
	int saveCount = 0;

	// Loop evolving the simplex
	const int maxIters = 20;
	int iter = 0;
	while (iter < maxIters)
	{
		// Copy current verticies
		saveCount = simplex.count;
		for (int i = 0; i < saveCount; ++i)
		{
			save0[i] = vertices[i].index0;
			save1[i] = vertices[i].index1;
		}

		simplex.FindNearestToOrigin();

		// If interior to polygon, end
		if (simplex.count == 3)
		{
			break;
		}

		// Get search direction.
		Vec<float> d = simplex.GetSearchDirection();

		// Ensure the search direction non-zero.
		if (d.GetLengthSq() == 0.0f)
		{
			break;
		}

		// New Vertex to add to the Simplex
		SimplexVertex* newvert = vertices + simplex.count;
		newvert->index0 = FindSupport(-d, model0);
		newvert->index1 = FindSupport(d, model1);
		newvert->point0 = model0[newvert->index0];
		newvert->point1 = model1[newvert->index1];
		newvert->point = newvert->point1 - newvert->point0;

		++iter;

		// Primary escape criterion: repeated support point
		bool duplicate = false;
		for (int i = 0; i < saveCount; ++i)
		{
			if (newvert->index0 == save0[i] && newvert->index1 == save1[i])
			{
				return simplex.PrepareResult(iter, 0, 0);
			}
		}

		// New vertex is ok- Solve functions always reduce the dimension of the simplex, so this is safe
		++simplex.count;
	}

	return simplex.PrepareResult(iter, 0, 0);
}

int Shapes::FindSupport(const Vec<float>& d, const std::vector<Vec<float>>& model)
{
	int furthestIndex = 0;
	float maxDist = model[0].Dot(d);
	for (int i = 1; i < int(model.size()); ++i)
	{
		float dist = model[i].Dot(d);
		if (dist > maxDist)
		{
			furthestIndex = i;
			maxDist = dist;
		}
	}

	return furthestIndex;
}

/// <summary>
/// Calculates the Axes-Aligned Bounding Box for a polygonal model.
/// </summary>
/// <param name="polyVerts"></param>
/// <returns>AABB for polygon</returns>
Rect<float> Shapes::BoundingBox(const std::vector<Vec<float>>& polyVerts)
{
	//Axes-Aligned Bounding Box
	Rect<float> AABB = Rect<float>(polyVerts[0], polyVerts[0]);
	for (int i=1; i<polyVerts.size(); ++i)
	{
		auto v = polyVerts[i];
		if (v.X < AABB.X0)
		{
			AABB.X0 = v.X;
		}
		else if (v.X > AABB.X1)
		{
			AABB.X1 = v.X;
		}

		if (v.Y < AABB.Y0)
		{
			AABB.Y0 = v.Y;
		}
		else if (v.Y > AABB.Y1)
		{
			AABB.Y1 = v.Y;
		}
	}

	return std::move(AABB);
}
