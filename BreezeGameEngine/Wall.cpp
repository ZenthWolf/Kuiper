#include "Wall.h"

Wall::Wall(RectF boundary, float thickness, Color c)
{
	Bound = boundary;
	borderThickness = thickness;
	C = c;
}
