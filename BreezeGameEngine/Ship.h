#pragma once

#include "Entity.h"
#include "Shapes.h"

class Ship : public Entity
{
public:
	Ship(Vec<float> pos = { 0.0f, 0.0f }, Color c = Colors::Cyan)
		:Entity(Shapes::Ship(), pos, c)
	{}
};