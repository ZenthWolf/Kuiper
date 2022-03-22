#define NOMINMAX // Needed to make min work
#include <algorithm>
#include "Tetra.h"
#include "Engine/Vec.h"



void Tetrahedron::Update(float dt)
{
	rotX += dt * .45;
	rotZ += dt * .7;

	if (rotX > 6.2831853)
		rotX -= 6.2831853;
	else if (rotX < -6.2831853)
		rotX += 6.2831853;

	if (rotZ > 6.2831853)
		rotZ -= 6.2831853;
	else if (rotZ < -6.2831853)
		rotZ += 6.2831853;

	pos.Z -= dt * 5;
	if (pos.Z < 0.0f)
		pos.Z = 50.0f;
}

void Tetrahedron::Draw(Graphics& gfx, Rect<float> cambox)
{
	pos.Z = forcedDist;
	TransformModel();

	for (int i = 0; i < 4; ++i)
	{
		float invDepth = 1./(vert[i].Z + additionalOffset);
		point[i] = Vec<float>(vert[i].X * screenDist * invDepth + (float)(cambox.width()) / 2, vert[i].Y * screenDist * invDepth + (float)(cambox.height()) / 2);
	}
	//                           01,    02,   03,   12,   13,  23
	float edgebrightness[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	
	//This is dumb as rocks for a real implementation
	float* zBuff = new float[gfx.ScreenHeight * gfx.ScreenWidth];
	for (int i = 0; i < gfx.ScreenHeight * gfx.ScreenWidth; ++i)
	{
		zBuff[i] = 0;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (norm[i].Z < 0)
		{
			float brightness = std::max(norm[i].Dot(Vec3<float>(1, -1, -2).Norm()), 0.15f);
			Color shade = Color(c.GetR() * brightness, c.GetG() * brightness, c.GetB() * brightness);
			Color line = Color(255 * brightness, 255 * brightness, 255 * brightness);
			switch (i)
			{
			case 0:
			{
				Color c = { 255,0,0 };
				Color shade = Color(c.GetR() * brightness, c.GetG() * brightness, c.GetB() * brightness);
				DrawTriFromLowest(point[0], point[1], point[2], shade, gfx);

				if (edgebrightness[0] < brightness)
				{
					edgebrightness[0] = brightness;
					gfx.DrawLine(point[0], point[1], line);
				}
				if (edgebrightness[1] < brightness)
				{
					edgebrightness[1] = brightness;
					gfx.DrawLine(point[0], point[2], line);
				}
				if (edgebrightness[3] < brightness)
				{
					edgebrightness[3] = brightness;
					gfx.DrawLine(point[1], point[2], line);
				}

				break;
			}
			case 1:
			{
				Color c = { 0,255,0 };
				Color shade = Color(c.GetR() * brightness, c.GetG() * brightness, c.GetB() * brightness);
				DrawTriFromLowest(point[0], point[1], point[3], shade, gfx);

				if (edgebrightness[2] < brightness)
				{
					edgebrightness[2] = brightness;
					gfx.DrawLine(point[0], point[3], line);
				}
				if (edgebrightness[0] < brightness)
				{
					edgebrightness[0] = brightness;
					gfx.DrawLine(point[0], point[1], line);
				}
				if (edgebrightness[4] < brightness)
				{
					edgebrightness[4] = brightness;
					gfx.DrawLine(point[3], point[1], line);
				}

				break;
			}
			case 2:
			{
				Color c = { 0,0,255 };
				Color shade = Color(c.GetR() * brightness, c.GetG() * brightness, c.GetB() * brightness);
				DrawTriFromLowest(point[0], point[3], point[2], shade, gfx);

				if (edgebrightness[1] < brightness)
				{
					edgebrightness[1] = brightness;
					gfx.DrawLine(point[0], point[2], line);
				}
				if (edgebrightness[2] < brightness)
				{
					edgebrightness[2] = brightness;
					gfx.DrawLine(point[0], point[3], line);
				}
				if (edgebrightness[5] < brightness)
				{
					edgebrightness[5] = brightness;
					gfx.DrawLine(point[2], point[3], line);
				}

				break;
			}
			case 3:
			{
				DrawTriFromLowest(point[1], point[2], point[3], shade, gfx);
				
				if (edgebrightness[4] < brightness)
				{
					edgebrightness[4] = brightness;
					gfx.DrawLine(point[3], point[1], line);
				}
				if (edgebrightness[5] < brightness)
				{
					edgebrightness[5] = brightness;
					gfx.DrawLine(point[2], point[3], line);
				}
				if (edgebrightness[3] < brightness)
				{
					edgebrightness[3] = brightness;
					gfx.DrawLine(point[1], point[2], line);
				}

				break;
			}
			default:
				break;
			}
			
		}
	}
	delete[] zBuff;
}

void  Tetrahedron::DrawTriFromLowest(Vec<float>& v0, Vec<float>& v1, Vec<float>& v2, Color c, Graphics& gfx)
{
	if (v0.Y < v1.Y)
	{
		if (v0.Y < v2.Y)
		{
			if(v1.Y < v2.Y)
				DrawTri(v0, v1, v2, c, gfx);
			else
				DrawTri(v0, v2, v1, c, gfx);
			return;
		}
		DrawTri(v2, v0, v1, c, gfx);
		return;
	}
	else if (v1.Y < v2.Y)
	{
		if(v0.Y < v2.Y)
			DrawTri(v1, v0, v2, c, gfx);
		else
			DrawTri(v1, v2, v0, c, gfx);
		return;
	}
	DrawTri(v2, v1, v0, c, gfx);
	return;
}

void Tetrahedron::AdjustScreenDist(const float dr)
{
	screenDist += dr;
}

void Tetrahedron::AdjustAdditionalOffset(const float dr)
{
	additionalOffset += dr;
}

void Tetrahedron::RotX(const float dth)
{
	rotX += dth;

	if (rotX > 6.2831853)
		rotX -= 6.2831853;
	else if (rotX < -6.2831853)
		rotX += 6.2831853;
}

void Tetrahedron::RotZ(const float dth)
{
	rotZ += dth;

	if (rotZ > 6.2831853)
		rotZ -= 6.2831853;
	else if (rotZ < -6.2831853)
		rotZ += 6.2831853;
}

Vec3<float> Tetrahedron::GetNorm(const int i)
{
	return norm[i];
}

void Tetrahedron::DrawTri(Vec<float>&v0, Vec<float>&v1, Vec<float>&v2, Color c, Graphics & gfx)
{
	//Different approach for CCW and CW orientations.
	if ((v1 - v0).Cross(v2 - v0) > 0)
	{
		float miny = v0.Y > 0 ? v0.Y : 0;
		float maxy = v1.Y < Graphics::ScreenHeight ? v1.Y : Graphics::ScreenHeight;

		for (float jf = miny; jf < maxy; jf+=1.0f)
		{
			int minx = std::max(0,(int)(v0.X + 1 + (v2.X - v0.X) * (jf - v0.Y) / (v2.Y - v0.Y)));
			int maxx = std::min(Graphics::ScreenWidth,(int)(v0.X + 1 + (v1.X - v0.X) * (jf - v0.Y) / (v1.Y - v0.Y)));

			for (int i = minx; i < maxx; ++i)
				gfx.PutPixel(i, (int)jf, c);
		}

		float jf = v1.Y;
		if (jf >= 0 && jf < float(Graphics::ScreenHeight))
		{
			int minx = std::max(0,(int)(v0.X + 1 + (v2.X - v0.X) * (jf - v0.Y) / (v2.Y - v0.Y)));
			int maxx = std::min(Graphics::ScreenWidth,(int)v1.X + 1);
			for (int i = minx; i < maxx; ++i)
				gfx.PutPixel(i, (int)jf, c);
		}

		miny = v1.Y > 0 ? v1.Y : 0;
		maxy = v2.Y < (float)Graphics::ScreenHeight ? v2.Y : (float)Graphics::ScreenHeight;
		for (float jf = miny; jf < maxy; jf+=1.0f)
		{
			int minx = std::max(0,(int)(v0.X + 1 + (v2.X - v0.X) * (jf - v0.Y) / (v2.Y - v0.Y)));
			int maxx = std::min(Graphics::ScreenWidth,(int)(v1.X + 1 + (v2.X - v1.X) * (jf - v1.Y) / (v2.Y - v1.Y)));

			for (int i = minx; i < maxx; ++i)
				gfx.PutPixel(i, (int)jf, c);
		}
	}
	else
	{
		float miny = v0.Y > 0 ? v0.Y : 0;
		float maxy = v1.Y < (float)Graphics::ScreenHeight ? v1.Y : (float)Graphics::ScreenHeight;
		for (float jf = miny; jf < maxy; jf+=1.0f)
		{
			int maxx = std::min(Graphics::ScreenWidth, (int)(v0.X + 1 + (v2.X - v0.X) * (jf - v0.Y) / (v2.Y - v0.Y)));
			int minx = std::max(0,(int)(v0.X + 1 + (v1.X - v0.X) * (jf - v0.Y) / (v1.Y - v0.Y)));
			for (int i = minx; i < maxx; ++i)
				gfx.PutPixel(i, (int)jf, c);
		}

		float jf = v1.Y;
		if (jf >= 0 && jf < float(Graphics::ScreenHeight))
		{
			int maxx = std::min(Graphics::ScreenWidth, (int)(v0.X + 1 + (v2.X - v0.X) * (jf - v0.Y) / (v2.Y - v0.Y)));
			int minx = std::max(0, (int)v1.X + 1);
			for (int i = minx; i < maxx; ++i)
				gfx.PutPixel(i, (int)jf, c);
		}

		miny = v1.Y  > 0 ? v1.Y : 0;
		maxy = v2.Y < (float)Graphics::ScreenHeight ? v2.Y : (float)Graphics::ScreenHeight;
		for (float jf = miny; jf < maxy; jf+=1.0f)
		{
			int max = std::min(Graphics::ScreenWidth, (int)(v0.X + 1 + (v2.X - v0.X) * (jf - v0.Y) / (v2.Y - v0.Y)));
			int min = std::max(0, (int)(v1.X + 1 + (v2.X - v1.X) * (jf - v1.Y) / (v2.Y - v1.Y)));
			for (int i = min; i < max; ++i)
				gfx.PutPixel(i, (int)jf, c);
		}
	}
}

void Tetrahedron::TransformModel()
{
	for (int i = 0; i < 4; ++i)
	{
		vert[i] = model[i] * (Matrix::XRotor(rotX) * Matrix::ZRotor(rotZ));
		vert[i] += pos;
		norm[i] = modnorm[i] * (Matrix::XRotor(rotX) * Matrix::ZRotor(rotZ));
	}
}
