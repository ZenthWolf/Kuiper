#pragma once
#include<cmath>
#include "Graphics/Graphics.h"
#include "Engine/Rect.h"

class Matrix
{
public:
	double m00, m01, m02;
	double m10, m11, m12;
	double m20, m21, m22;

	Matrix(double e00, double e01, double e02,
		double e10, double e11, double e12,
		double e20, double e21, double e22)
	{
		m00 = e00; m01 = e01; m02 = e02;
		m10 = e10; m11 = e11; m12 = e12;
		m20 = e20; m21 = e21; m22 = e22;
	}

	Matrix operator *(Matrix N)
	{
		double new00 = m00 * N.m00 + m01 * N.m10 + m02 * N.m20;
		double new01 = m00 * N.m01 + m01 * N.m11 + m02 * N.m21;
		double new02 = m00 * N.m02 + m01 * N.m12 + m02 * N.m22;

		double new10 = m10 * N.m00 + m11 * N.m10 + m12 * N.m20;
		double new11 = m10 * N.m01 + m11 * N.m11 + m12 * N.m21;
		double new12 = m10 * N.m02 + m11 * N.m12 + m12 * N.m22;

		double new20 = m20 * N.m00 + m21 * N.m10 + m22 * N.m20;
		double new21 = m20 * N.m01 + m21 * N.m11 + m22 * N.m21;
		double new22 = m20 * N.m02 + m21 * N.m12 + m22 * N.m22;

		return Matrix(new00, new01, new02,
			new10, new11, new12,
			new20, new21, new22);
	}

	static Matrix XRotor(double th)
	{
		return Matrix(1, 0, 0,
			0, cosf(th), sinf(th),
			0, -sinf(th), cosf(th));
	}

	static Matrix YRotor(double th)
	{
		return Matrix(cosf(th), 0, sinf(th),
			0, 1, 0,
			0, -sinf(th), cosf(th));
	}

	static Matrix ZRotor(double th)
	{
		return Matrix(cosf(th), sinf(th), 0,
			-sinf(th), cosf(th), 0,
			0, 0, 1);
	}
};

template<typename T>
class Vec3
{
public:
	Vec3()
	{
		X = T(0);
		Y = T(0);
		Z = T(0);
	}

	Vec3(T x, T y, T z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	T GetLength() const
	{
		return std::sqrt(GetLengthSq());
	}

	T GetLengthSq() const
	{
		return X * X + Y * Y + Z * Z;
	}

	Vec3 operator+(const Vec3& rhs) const
	{
		return Vec3(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
	}

	Vec3& operator+=(const Vec3& rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		Z += rhs.Z;
		return *this;
	}

	bool operator==(const Vec3& rhs)
	{
		return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
	}

	Vec3 operator-(const Vec3& rhs) const
	{
		return Vec3(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
	}

	Vec3 operator-() const
	{
		return Vec3<T>(-X, -Y, -Z);
	}

	Vec3& operator-=(const Vec3& rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		Z -= rhs.z;
		return *this;
	}

	Vec3 operator*(const T scale) const
	{
		return Vec3<T>(X * scale, Y * scale, Z * scale);
	}

	Vec3 operator *=(const T scale)
	{
		X *= scale;
		Y *= scale;
		Z *= scale;
		return *this;
	}

	Vec3 operator/(const T scale) const
	{
		return Vec3(X / scale, Y / scale, Z / scale);
	}

	Vec3 Norm() const
	{
		if (GetLength() != (T)0)
		{
			float mag = GetLength();
			return { X / mag, Y / mag, Z / mag };
		}
		else
		{
			return { 0.0f, 0.0f, 0.0f };
		}
	}

	T Dot(const Vec3<T> rhs) const
	{
		return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
	}

	Vec3<T> Cross(const Vec3<T> rhs) const
	{
		return {Y * rhs.Z - Z * rhs.Y, Z * rhs.X - X * rhs.Z , X * rhs.Y - Y * rhs.X };
	}

	Vec3<T> operator *(Matrix M)
	{
		return Vec3(X * M.m00 + Y * M.m10 + Z * M.m20,
			X * M.m01 + Y * M.m11 + Z * M.m21,
			X * M.m02 + Y * M.m12 + Z * M.m22);
	}

	template<typename U>
	explicit Vec3(const Vec3<U> src)
	{
		X = (T)src.X;
		Y = (T)src.Y;
		Z = (T)srx.Z;
	}

public:
	T X;
	T Y;
	T Z;
};

class Tetrahedron
{
public:
	float screenDist = 320.f;
	float additionalOffset = 5.0f;
	float forcedDist = 20.0f;

	Tetrahedron(const float size)
	{
		pos = { 0.0f, 0.0f, 50.0f };
		model[0] = Vec3<float>(1, 0, -1 / sqrtf(2)) * (size * 0.5f);
		model[1] = Vec3<float>(-1, 0, -1 / sqrtf(2)) * (size * 0.5f);
		model[2] = Vec3<float>(0, 1, 1 / sqrtf(2)) * (size * 0.5f);
		model[3] = Vec3<float>(0, -1, 1 / sqrtf(2)) * (size * 0.5f);

		modnorm[0] = (model[0] - model[1]).Cross(model[0] - model[2]);
		modnorm[1] = (model[0] - model[1]).Cross(model[0] - model[3]);
		modnorm[2] = (model[0] - model[2]).Cross(model[0] - model[3]);
		modnorm[3] = (model[1] - model[2]).Cross(model[1] - model[3]);

		if (modnorm[0].Dot(model[3] - model[0]) > 0.0f)
			modnorm[0] = -(modnorm[0].Norm());
		else
			modnorm[0] = (modnorm[0].Norm());

		if (modnorm[1].Dot(model[2] - model[0]) > 0.0f)
			modnorm[1] = -(modnorm[1].Norm());
		else
			modnorm[1] = (modnorm[1].Norm());

		if (modnorm[2].Dot(model[1] - model[0]) > 0.0f)
			modnorm[2] = -(modnorm[2].Norm());
		else
			modnorm[2] = (modnorm[2].Norm());

		if (modnorm[3].Dot(model[0] - model[1]) > 0.0f)
			modnorm[3] = -(modnorm[3].Norm());
		else
			modnorm[3] = (modnorm[3].Norm());
	}

	void Update(float dt);
	void Draw(Graphics& gfx, Rect<float> cambox);
	void AdjustScreenDist(float dr);
	void AdjustAdditionalOffset(float dr);

private:
	void TransformModel();
	void DrawTriFromLowest(Vec<float>& v0, Vec<float>& v1, Vec<float>& v2, Color c, Graphics& gfx);
	void DrawTri(Vec<float>& v0, Vec<float>& v1, Vec<float>& v2, Color c, Graphics& gfx);

	Vec3<float> model[4];
	Vec3<float> modnorm[4];
	Vec3<float> vert[4];
	Vec3<float> norm[4];

	Vec<float> point[4];

	Vec3<float> pos;
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;
	const Color c = Color(80, 30, 255);
};
