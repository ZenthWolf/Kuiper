#pragma once
#include "Graphics.h"
#include "Vec.h"
#include "RectF.h"

class Ball 
{
public:
	Ball();
	Ball(float x, float y, Color c);
	Ball(float x, float y, float vx, float vy, Color c);
	void Draw(Graphics& gfx);
	void Move(const float dt);
	bool CollWall(const RectF& wall);
	RectF GetRect() const;
	void BounceX();
	void BounceY();
	void Bounce(const Vec impulse);
	void BounceY(const float impy);
	Vec GetPos() const;
	Vec GetVel() const;

private:
	Vec Pos;
	Vec Vel;	
	static constexpr float R = 10.0f;
	Color C;
};