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

#include "Entity.h"
#include "Shapes.h"

class Asteroid : public Entity
{
public:
	Asteroid(Vec<float> pos = { 0.0f, 0.0f }, Vec<float> vel = { 0.0f, 0.0f }, float rot = 0.0f, Color c = Colors::LightGrey)
		:Entity(Shapes::Asteroid(), pos, vel, rot, c)
	{
	}

	void Update(const float dt)
	{
		staleModel = true;
		stalePrimitives = true;
		TranslateBy(vel * dt);
		RotBy(angvel * dt);
	}
};