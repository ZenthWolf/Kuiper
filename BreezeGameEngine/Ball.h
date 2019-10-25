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
	void Draw(Graphics& gfx);
	void Move(const float dt);
	bool CollWall(const RectF& wall);
	RectF GetRect() const;
	void BounceX();
	void BounceY();
	void BounceY(const Vec offx);
	void Bounce(const Vec impulse);
	Vec GetPos() const;
	Vec GetVel() const;
	void SetDir(const Vec Dir);

private:
	Vec Pos;
	Vec Vel;
	float speed;
	static constexpr float R = 8.0f;
	Color C;
};