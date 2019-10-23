#pragma once
#include "Graphics.h"

class Rect 
{
public:
	Rect(Graphics& gfx);
	Rect(Graphics& gfx, float x, float y, float w, float h, Color c);
	float X;
	float Y;
	float W;
	float H;
	Color C;
	void Draw();

private:
	Graphics& gfx;
};