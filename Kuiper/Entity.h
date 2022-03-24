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
#include<memory>

#include "Engine/BreezeException.h"

//Forward Declarations
struct ActiveEdge;

struct LastColl
{
	Vec<float> pos;
	Vec<float> vel;
	float heading;
	float angvel;
	std::vector<Vec<float>> model;
	std::vector<std::vector<Vec<float>>> primitives;
};



class Entity
{
public:
	Entity(std::vector<Vec<float>> modl, const Vec<float>& pos, const Vec<float>& vel, float angvel, Color c);

	//Parameters
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
	void SetAngVel(const float newvel);
	Rect<float> GetBoundingBox() const;

	//Rendering
	Drawable GetDrawable(bool debug = false) const;
	std::list<Drawable> GetDrawList() const;
	void SetColor(const Color cnew);

	//Interactions
	void Recoil(const ActiveEdge& contactEdge, Entity& targ, const float rewindTime, int&, bool);
	bool CollPoint(const Vec<float> targ) const;
	Vec<float> GetTransformedVertex(const int vert) const;
	std::vector<Vec<float>> GetTransformedModel() const;
	std::vector<std::vector<Vec<float>>> GetTransformedPrimitives() const;
	Vec<float> Entity::UntransformPoint(const Vec<float> pnt);

	//History Recording
	void SetHistory();
	LastColl ReadHistory() const;
	void ResetHistory();
	void alertStaleModel() const;

	bool didColl = false;

	enum class CollDepth
	{
		Free = 0,
		FarField = 1,
		MidField = 2,
		Collided = 3
	};
	mutable CollDepth depth = CollDepth::Free;

protected:
	float ClusterArea(const Vec<float> A, const Vec<float> B, const Vec<float> C) const;

	std::vector<Vec<float>> model;
	std::vector<std::vector<Vec<float>>> modelprimitives;
	mutable std::vector<Vec<float>> cachedModel;
	mutable bool staleModel = true;
	mutable std::vector<std::vector<Vec<float>>> cachedPrimitives;
	mutable bool stalePrimitives = true;

	Color c;

	Vec<float> pos = { 0.0f, 0.0f };
	Vec<float> vel = { 0.0f, 0.0f };
	float vmax = 200.0f;

	float heading = 0.0f;
	float angvel = 0.0f;
	float angvelMax = 3.0f;

	float scale = 1.0f;
	float boundingrad = 0.0f;

	LastColl History;
};
