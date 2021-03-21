#pragma once

#include <vector>
#include "Vec.h"
#include "Colors.h"
#include "Drawable.h"

class Entity
{
public:
	Entity(std::vector<Vec<float>> modl, const Vec<float>& pos, const Vec<float>& vel, float rot, Color c);

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

	bool CollWith(Entity targ);
	std::vector<Vec<float>> GetTransformedModel() const;
	void Recoil(Entity& targ);

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