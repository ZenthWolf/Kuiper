#pragma once
#pragma once

#include "Entity.h"
#include "Shapes.h"

class Star : public Entity
{
public:
	Star(Vec<float> pos = { 0.0f, 0.0f }, Color c = Colors::Yellow)
		:Entity(Shapes::Star(), pos, c)
	{}
};