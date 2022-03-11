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

#pragma once

#include <vector>
#include <list>
#include "Engine/Vec.h"
#include "Graphics/Colors.h"
#include "Graphics/Drawable.h"

#include "Engine/BreezeException.h"

//Forward Declarations
struct ActiveEdge;

struct LastColl
{
	Vec<float> pos;
	Vec<float> vel;
	float heading;
	float rot;
	std::vector<Vec<float>> model;
	std::vector<std::vector<Vec<float>>> primitives;
};



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

	Drawable GetDrawable(bool debug = false) const;
	std::list<Drawable> GetDrawList() const;
	void SetColor(const Color cnew);

	Rect<float> GetBoundingBox() const;
	Rect<float> GetBoundingBox(const std::vector<Vec<float>>& model) const;

	struct CollInfo
	{
		float impactDepth;
		Vec<int> impactSide;
	};

	std::vector<int> CollWith(const Entity& targ) const;
	void Recoil(ActiveEdge*& contactEdge, Entity& targ);

	bool CollPoint(const Vec<float> targ) const;
	Vec<float> GetTransformedVertex(const int vert) const;
	std::vector<Vec<float>> GetTransformedModel() const;
	std::vector<std::vector<Vec<float>>> GetTransformedPrimitives() const;
	Vec<float> Entity::UntransformPoint(const Vec<float> pnt);
	
	CollInfo CalculateImpact(const Vec<float> point, const Vec<float> Velocity) const;

	void SetHistory();
	LastColl ReadHistory();
	void ResetHistory();

	bool didColl = false;
public:
	float rot = 0.0f;
	enum class CollDepth
	{
		Free = 0,
		FarField = 1,
		MidField = 2,
		Collided = 3
	};

	mutable CollDepth depth = CollDepth::Free;
protected:
	void SetModel(std::vector<Vec<float>> modelnew);

	float ClusterArea(const Vec<float> A, const Vec<float> B, const Vec<float> C) const;

	std::vector<Vec<float>> model;
	std::vector<std::vector<Vec<float>>> modelprimitives;

	Color c;

	Vec<float> pos = { 0.0f, 0.0f };
	Vec<float> vel = { 0.0f, 0.0f };
	float vmax = 200.0f;

	float heading = 0.0f;
	//float rot = 0.0f;
	float rotmax = 3.0f;

	float scale = 1.0f;
	float boundingrad = 0.0f;

	LastColl History;
};
