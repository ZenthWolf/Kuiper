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

#include "Surface.h"
#include "Graphics.h"
#include <vector>

class Animation
{
public:
	Animation( int x0, int y0, int width, int height, int framecount,
		       const Surface& sprite, float holdTime, Color chroma = Colors::Magenta);
	void Update(float dt);
	void Draw(const Vec<int>& pos, Graphics& gfx, bool drawAll) const;
	void Draw(const Vec<int>& pos, Graphics& gfx, Color sub) const;
	void Draw(const Vec<int>& pos, Graphics& gfx, Rect<int>& clip) const;
private:
	void Advance();

	Color chromakey;
	const Surface& sprite;
	std::vector<Rect<int>> frame;
	int iCurFrame = 0;
	float holdTime;
	float frameTime = 0.0f;
};
