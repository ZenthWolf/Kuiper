/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#include "Graphics/Camera.h"

Camera::Camera(CoordinateTransformer& ct)
	:ct(ct)
{}



Vec<float> Camera::GetPos() const
{
	return pos;
}

void Camera::MoveBy(const Vec<float>& offset)
{
	pos += offset;
}

void Camera::MoveTo(const Vec<float>& newpos)
{
	pos = newpos;
}

float Camera::GetScale() const
{
	return scale;
}

void Camera::SetScale(const float s)
{
	scale = s;
}



Vec<float> Camera::TransformToRealSpace(Vec<float> pnt) const
{
	pnt -= { float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight / 2) };
	pnt.Y *= -1;
	pnt *= 1 / scale;
	pnt += pos;

	return pnt;
}



Rect<float> Camera::GetScreenBox() const
{
	const float zoom = 1.0f / scale;
	return Rect<float>::FromCent(pos,
		float(Graphics::ScreenWidth / 2) * zoom,
		float(Graphics::ScreenHeight / 2) * zoom);
}

void Camera::Draw(Drawable& drawable, bool debugJoints) const
{
	drawable.Translate(-pos);
	drawable.Scale(scale);
	ct.Draw(drawable, debugJoints);
}

void Camera::Draw(std::list<Drawable>& drawable) const
{
	for (auto d : drawable)
	{
		Draw(d);
	}
}