#pragma once

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
	
	void Draw(Drawable& drawable) const;

private:
	CoordinateTransformer& ct;

	Vec<float> pos = { 0.0f, 0.0f };
	float scale = 1.0f;
};