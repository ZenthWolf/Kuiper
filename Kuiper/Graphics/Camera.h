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

#include<list>
#include "CoordinateTransformer.h"

class Camera
{
public:
	Camera(CoordinateTransformer& ct);

	Vec<float> GetPos() const;
	void MoveBy(const Vec<float>& offset);
	void MoveTo(const Vec<float>& newpos);
	float GetScale() const;
	void SetScale(const float s);

	Vec<float> TransformToRealSpace(const Vec<float> pnt) const;

	Rect<float> GetScreenBox() const;
	
	void Draw(Drawable& drawable, bool debugJoints = true) const;
	void Draw(std::list<Drawable>& drawable) const;

private:
	CoordinateTransformer& ct;

	Vec<float> pos = { 0.0f, 0.0f };
	float scale = 1.0f;
};