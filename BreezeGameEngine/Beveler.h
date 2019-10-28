#pragma once

#include "Graphics.h"
#include "RectI.h"

class Beveler
{
public:
	Beveler() = default;
	Beveler(Color base);
	void DrawOuterBev(RectI& Rect, int size, Graphics& gfx);
	void DrawInnerBev(RectI& Rect, int size, Graphics& gfx);
	void DrawBevBrick(RectI& Rect, int size, Graphics& gfx);
	void DrawBevBorder(RectI& Rect, int size, Graphics& gfx);
	void ChangeBaseColor(Color newbase);
private:
	static constexpr float LeftFactor = 1.10f;
	static constexpr float RightFactor = 0.93f;
	static constexpr float TopFactor = 0.80f;
	static constexpr float BotFactor = 0.65f;
	Color BaseColor;
	Color LeftColor;
	Color RightColor;
	Color TopColor;
	Color BotColor;
};
