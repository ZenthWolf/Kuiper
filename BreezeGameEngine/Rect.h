#pragma once
#include "Graphics.h"
#include "Vec.h"

class Rect 
{
public:
	Rect(Graphics& gfx);
	Rect(Graphics& gfx, float x, float y, float w, float h, Color c);
	Vec Pos;
	float W;
	float H;
	Color C;
	void Draw();
	bool CollWith( const Rect& Targ ) const;

private:
	Graphics& gfx;
};