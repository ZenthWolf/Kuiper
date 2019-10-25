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
	bool CollBall(Ball& ball) const;
	void CollWall(RectF& wall);
	RectF GetRect() const;

private:
	static constexpr float wingWidth = 15.0f;
	Color wingC = Colors::Gray;
	static constexpr float Vx = 3.0f;
	Color C = Colors::LightGray;
	Vec Pos;
	float halfWidth;
	float halfHeight;
	static int Tests;
};