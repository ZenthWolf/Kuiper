#pragma once

#include "RectF.h"
#include "Colors.h"

class Wall 
{
public:
	Wall(RectF boundary, float thickness, Color c);
RectF Bound;
private:
	
	float borderThickness;
	Color C;
};