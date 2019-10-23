#pragma once
#include "Graphics.h"

class Ball 
{
public:
	Ball(Graphics& gfx);
	Ball(Graphics& gfx, float x, float y, Color c);
	Ball(Graphics& gfx, float x, float y, float vx, float vy, Color c);
	void Draw();

private:
	float X;
	float Y;
	float Vx;
	float Vy;
	static constexpr int R = 17;
	Color C;
	Graphics& gfx;
};