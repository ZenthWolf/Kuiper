#pragma once

#include "Rect.h"
#include "Graphics.h"
#include "Keyboard.h"

class Paddle 
{
public:
	Paddle(Graphics& gfx, Keyboard& kbd);
	Paddle(Graphics& gfx, Keyboard& kbd, float x, float y, float w, float h, Color c);
	void Draw();
	void Update(Keyboard& kbd);

private:
	Graphics& gfx;
	Rect Rect;
	float Vx = 0;

	Keyboard& kbd;
};