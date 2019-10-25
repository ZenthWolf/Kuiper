#pragma once

#include "RectF.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Ball.h"

class Paddle 
{
public:

	Paddle(Vec pos, float hWidth, float hHeight);
	void Draw(Graphics& gfx) const;
	void Move(const Keyboard& kbd, const float dt);
	bool checkColl(Ball& ball) const;
	bool CollBall(Ball& ball) ;
	void CollWall(RectF& wall);
	RectF GetRect() const;
	void Cool();

private:
	static constexpr float wingWidth = 15.0f;
	Color wingC = Colors::Gray;
	static constexpr float Vx = 7.0f;
	Color C = Colors::LightGray;
	Vec Pos;
	float halfWidth;
	float halfHeight;
	bool onCooldown = false;
};