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
	RectF Rect;

private:
	Graphics& gfx;
	
	static constexpr float Vx = 3.0f;

	Keyboard& kbd;
};