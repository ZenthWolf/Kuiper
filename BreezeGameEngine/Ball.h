#pragma once
#include "Graphics.h"
#include "Vec.h"
#include "RectF.h"

class Ball 
{
public:
	Ball();
	Ball(float x, float y, Color c);
	Ball(float x, float y, float speed, Color c);
	Ball(float x, float y, float speed, Vec initDir, Color c);
	Ball(Vec pos, float speed, Vec initDir, Color c);
	void Draw(Graphics& gfx);
	void Move(const float dt);
	bool CollWall(const RectF& wall);
	RectF GetRect() const;
	void BounceX();
	void BounceY();
	void BounceY(const Vec offx);
	Vec GetPos() const;
	Vec GetVel() const;
	void SetDir(const Vec Dir);
	float GetRad() const;

private:
	Vec Pos;
	Vec Vel;
	float speed;
	static constexpr float R = 6.0f;
	Color C;
};