#include <cassert>
#include "Shapes.h"

/*
In Polygon-Polygon collisions, this "Simplex" is that of
the Minkowski Difference generated polygon
*/

Vec<float> Simplex::GetSearchDirection() const
{
	switch (count)
	{
	case 1:
		return -vertex0.point;

	case 2:
	{
		Vec<float> edge = vertex1.point - vertex0.point;
		float sgn = edge.Cross(-vertex0.point);
		if (sgn > 0.0f)
		{
			// Origin is left of edge.
			return Vec<float>{-edge.Y, edge.X};
		}
		else
		{
			// Origin is right of edge.
			return Vec<float>{edge.Y, -edge.X};
		}
	}

	default:
		assert(false);
		return Vec<float>(0.0f, 0.0f);
	}
}

//Find closest point on this 1-Plex to origin (indicative of nearest point between polys)
void Simplex::Solve2()
{
	Vec<float> A = vertex0.point;
	Vec<float> B = vertex1.point;

	//unnormalized barycentric coordinates
	float u = -B.Dot(A - B);
	float v = -A.Dot(B - A);

	// Region A
	if (v <= 0.0f)
	{
		// New Simplex is vertex A.
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region B
	if (u <= 0.0f)
	{
		// New Simplex is vertex B, which needs to be moved.
		vertex0 = vertex1;
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region AB. 
	vertex0.u = u;
	vertex1.u = v;
	Vec<float> e = B - A;
	//if Degenerate, previous region was called.
	divisor = e.GetLengthSq();
	count = 2;
}

//Find closest point on this 2-Plex to origin (indicative of nearest point between polys)
void Simplex::Solve3()
{
	Vec<float> A = vertex0.point;
	Vec<float> B = vertex1.point;
	Vec<float> C = vertex2.point;

	// Unnormalized edge barycentric coordinate
	float uAB = -B.Dot(A - B);
	float vAB = -A.Dot(B - A);

	float uBC = -C.Dot(B - C);
	float vBC = -B.Dot(C - B);

	float uCA = -A.Dot(C - A);
	float vCA = -C.Dot(A - C);

	//Check to for reduction to 0-Plex
	// Region A
	if (vAB <= 0.0f && uCA <= 0.0f)
	{
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region B
	if (uAB <= 0.0f && vBC <= 0.0f)
	{
		vertex0 = vertex1;
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Region C
	if (uBC <= 0.0f && vCA <= 0.0f)
	{
		vertex0 = vertex2;
		vertex0.u = 1.0f;
		divisor = 1.0f;
		count = 1;
		return;
	}

	// Signed triangle area.
	float area = (B - A).Cross(C - A);

	// Unnormalized triangular barycentric coordinates
	// With above, tests that pnt is exterior a particular side
	float uQBC = B.Cross(C);
	float vAQC = C.Cross(A);
	float wABQ = A.Cross(B);

	// Region AB
	if (uAB > 0.0f && vAB > 0.0f && wABQ * area <= 0.0f)
	{
		vertex0.u = uAB;
		vertex1.u = vAB;
		Vec<float> e = B - A;
		divisor = e.Dot(e);
		count = 2;
		return;
	}

	// Region BC
	if (uBC > 0.0f && vBC > 0.0f && uQBC * area <= 0.0f)
	{
		vertex0 = vertex1;
		vertex1 = vertex2;

		vertex0.u = uBC;
		vertex1.u = vBC;
		Vec<float> e = C - B;
		divisor = e.GetLengthSq();
		count = 2;
		return;
	}

	// Region CA
	if (uCA > 0.0f && vCA > 0.0f && vAQC * area <= 0.0f)
	{
		//This is weird, investigate what swapping placement changes
		vertex1 = vertex0;
		vertex0 = vertex2;

		vertex0.u = uCA;
		vertex1.u = vCA;
		Vec<float> e = A - C;
		divisor = e.GetLengthSq();
		count = 2;
		return;
	}

	// Region ABC
	assert(abs(area) > 0.0f && uQBC * area > 0.0f && vAQC * area > 0.0f && wABQ * area > 0.0f);
	vertex0.u = uQBC;
	vertex1.u = vAQC;
	vertex2.u = wABQ;
	//Divisior non-zero, else previous region catches
	divisor = area;
	count = 3;
}

Approach Simplex::PrepareResult(int iter, int convex0, int convex1)
{
	float s = 1.0f / divisor;
	Approach result;

	result.convex0 = convex0;
	result.convex1 = convex1;

	switch (count)
	{
	case 1:
		result.point0 = vertex0.point0;
		result.index0 = vertex0.index0;
		result.type0 = Approach::Type::Vertex;

		result.point1 = vertex0.point1;
		result.index1 = vertex0.index1;
		result.type1 = Approach::Type::Vertex;
		break;

	case 2:
	{
		result.point0 = vertex0.point0 * (s * vertex0.u) + vertex1.point0 * (s * vertex1.u);

		if (abs(result.point0.X - vertex0.point0.X) < 0.001 && abs(result.point0.Y - vertex0.point0.Y) < 0.001)
		{
			result.index0 = vertex0.index0;
			result.type0 = Approach::Type::Vertex;
		}
		else if (abs(result.point0.X - vertex1.point0.X) < 0.001 && abs(result.point0.Y - vertex1.point0.Y) < 0.001)
		{
			result.index0 = vertex1.index0;
			result.type0 = Approach::Type::Vertex;
		}
		else
		{
			if ((vertex0.index0 < vertex1.index0) && (vertex0.index0 + 1 == vertex1.index0))
			{
				result.index0 = vertex0.index0;
			}
			else if ((vertex1.index0 < vertex0.index0) && (vertex1.index0 + 1 == vertex0.index0))
			{
				result.index0 = vertex1.index0;
			}
			else
			{
				result.index0 = std::max(vertex0.index0, vertex1.index0);
			}
			result.type0 = Approach::Type::Edge;
		}


		result.point1 = vertex0.point1 * (s * vertex0.u) + vertex1.point1 * (s * vertex1.u);
		if (abs(result.point1.X - vertex0.point1.X) < 0.001 && abs(result.point1.Y - vertex0.point1.Y) < 0.001)
		{
			result.index1 = vertex0.index1;
			result.type1 = Approach::Type::Vertex;
		}
		else if (abs(result.point1.X - vertex1.point1.X) < 0.001 && abs(result.point1.Y - vertex1.point1.Y) < 0.001)
		{
			result.index1 = vertex1.index1;
			result.type1 = Approach::Type::Vertex;
		}
		else
		{
			if ((vertex0.index1 < vertex1.index1) && (vertex0.index1 + 1 == vertex1.index1))
			{
				result.index1 = vertex0.index1;
			}
			else if ((vertex1.index1 < vertex0.index1) && (vertex1.index1 + 1 == vertex0.index1))
			{
				result.index1 = vertex1.index1;
			}
			else
			{
				result.index1 = std::max(vertex0.index1, vertex1.index1);
			}
			result.type1 = Approach::Type::Edge;
		}
	}
	break;

	case 3:
	{
		result.point0 = vertex0.point0 * (s * vertex0.u) + vertex1.point0 * (s * vertex1.u) + vertex2.point0 * (s * vertex2.u);
		result.point1 = vertex0.point1 * (s * vertex0.u) + vertex1.point1 * (s * vertex1.u) + vertex2.point1 * (s * vertex2.u);
		//result.point1 = result.point0;
	}
	break;

	default:
		assert(false);
		break;
	}

	result.distance = (result.point1 - result.point0).GetLength();

	result.iterations = iter;
	return std::move(result);
}
