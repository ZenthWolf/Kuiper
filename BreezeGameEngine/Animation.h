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
