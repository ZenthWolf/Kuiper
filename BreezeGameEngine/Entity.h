#pragma once

#include <vector>
#include "Vec.h"
#include "Colors.h"
#include "Drawable.h"

#include "BreezeException.h"


class Entity
{
public:
	Entity(std::vector<Vec<float>> modl, const Vec<float>& pos, const Vec<float>& vel, float rot, Color c);

	class ColliderException : public BreezeException
	{
	public:
		ColliderException(const std::wstring& note, const wchar_t* file, unsigned int line)
			:BreezeException(file, line, note)
		{}

		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	};

	enum class vertexType { outside, unchecked, inside };

	const Vec<float>& GetPos() const;
	void SetPos(const Vec<float> newpos);
	void TranslateBy(const Vec<float> dpos);
	void SetScale(const float s);
	float GetScale() const;
	float GetRadius() const;
	void SetHeading(const float th);
	void RotBy(const float th);
	Vec<float> GetHeading() const;

	Vec<float> GetVel() const;
	void SetVel(const Vec<float> newvel);

	Drawable GetDrawable() const;
	void SetColor(Color cnew);

	struct Intersection
	{
		std::vector <Vec<int>> sourceLinesIndex;
		std::vector <Vec<int>> targetLinesIndex;
		std::vector<Vec<float>> sourceTrans;
		std::vector<Vec<float>> targetTrans;
		int passed = 0;
	};

	Intersection CollWith(const Entity& targ);
	void Recoil(Intersection intersection, Entity& targ);
	std::vector<vertexType> ResolveInternalVertex(Intersection intersection, Entity& targ);
	Vec<float> MomentumTransfer(Vec<float> s0, Vec<float> t0, Vec<float>t1, Entity& targ);

	bool CollPoint(const Vec<float> targ);
	std::vector<Vec<float>> GetTransformedModel() const;
	Vec<float> Entity::UntransformPoint(const Vec<float> pnt);
	

protected:
	void SetModel(std::vector<Vec<float>> modelnew);

	float ClusterArea(Vec<float> A, Vec<float> B, Vec<float> C);

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