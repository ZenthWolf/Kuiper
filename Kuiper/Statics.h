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
#pragma once

#include "Entity.h"
#include "Shapes.h"

class Star : public Entity
{
public:
	Star(Vec<float> pos = { 0.0f, 0.0f }, Color c = Colors::Yellow)
		:Entity(Shapes::Star(), pos, { 0.0f, 0.0f }, 0.0f, c)
	{}
};