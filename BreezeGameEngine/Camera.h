#pragma once

#include "CoordinateTransformer.h"

class Camera
{
public:
	Camera( CoordinateTransformer& ct)
		:ct(ct)
	{}

	Vec<float> GetPos() const
	{
		return pos;
	}

	float GetScale() const
	{
		return scale;
	}

	void MoveBy(const Vec<float>& offset)
	{
		pos += offset;
	}

	void MoveTo(const Vec<float>& newpos)
	{
		pos = newpos;
	}

	void SetScale(const float s)
	{
		scale = s;
	}

	void Draw( Drawable& drawable) const
	{
		drawable.Translate(-pos);
		drawable.Scale(scale);
		ct.Draw( drawable );
	}

private:
	CoordinateTransformer& ct;

	Vec<float> pos = { 0.0f, 0.0f };
	float scale = 1.0f;
};