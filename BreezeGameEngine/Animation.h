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
	void Draw(const VecI& pos, Graphics& gfx) const;
	void Draw(const VecI& pos, Graphics& gfx, RectI& clip) const;
private:
	void Advance();

	Color chroma;
	const Surface& sprite;
	std::vector<RectI> frame;
	int iCurFrame = 0;
	float holdTime;
	float frameTime = 0.0f;
};