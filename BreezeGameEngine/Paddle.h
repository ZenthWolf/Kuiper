#pragma once

#include "RectF.h"
#include "Graphics.h"
#include "Keyboard.h"

class Paddle 
{
public:
	Paddle(Graphics& gfx, Keyboard& kbd);
	Paddle(Graphics& gfx, Keyboard& kbd, float x, float y, float w, float h, Color c);
	void Draw();
	void Update(Keyboard& kbd, float dt);

private:
	Graphics& gfx;
	RectF Rect;
	static constexpr float Vx = 1.5f;

	Keyboard& kbd;
};