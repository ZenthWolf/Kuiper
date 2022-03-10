/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#pragma once

#include "Graphics.h"
#include "../Engine/Rect.h"

class Beveler
{
public:
	Beveler() = default;
	Beveler(Color base);
	void DrawOuterBev(Rect<int>& Rect, int size, Graphics& gfx);
	void DrawInnerBev(Rect<int>& Rect, int size, Graphics& gfx);
	void DrawBevBrick(Rect<int>& Rect, int size, Graphics& gfx);
	void DrawBevBorder(Rect<int>& Rect, int size, Graphics& gfx);
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
