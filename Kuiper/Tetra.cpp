#define NOMINMAX // Needed to make min work
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
	TransformModel();
	
	const float screenDist = 320.f;

	for (int i = 0; i < 4; ++i)
	{
		float invDepth = 1./(vert[i].Z + 5.0f);
		point[i] = Vec<float>(vert[i].X * screenDist * invDepth + (float)(cambox.width()) / 2, vert[i].Y * screenDist * invDepth + (float)(cambox.height()) / 2);
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
				DrawTriFromLowest(point[0], point[1], point[2], shade, gfx);
				gfx.DrawLine(point[0], point[1], line);
				gfx.DrawLine(point[0], point[2], line);
				gfx.DrawLine(point[1], point[2], line);
				break;
			}
			case 1:
			{
				DrawTriFromLowest(point[0], point[1], point[3], shade, gfx);
				gfx.DrawLine(point[0], point[3], line);
				gfx.DrawLine(point[0], point[1], line);
				gfx.DrawLine(point[3], point[1], line);
				break;
			}
			case 2:
			{
				DrawTriFromLowest(point[0], point[3], point[2], shade, gfx);
				gfx.DrawLine(point[0], point[2], line);
				gfx.DrawLine(point[0], point[3], line);
				gfx.DrawLine(point[2], point[3], line);
				break;
			}
			case 3:
			{
				DrawTriFromLowest(point[1], point[2], point[3], shade, gfx);
				gfx.DrawLine(point[3], point[1], line);
				gfx.DrawLine(point[3], point[2], line);
				gfx.DrawLine(point[1], point[2], line);
				break;
			}
			default:
				break;
			}
			
		}
	}
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

void Tetrahedron::DrawTri(Vec<float>&v0, Vec<float>&v1, Vec<float>&v2, Color c, Graphics & gfx)
{
	if ((v1 - v0).Cross(v2 - v0) > 0)
	{
		for (int j = (int)v0.Y; j < (int)v1.Y; j++)
		{
			int min = (int)v0.X + (v2.X - v0.X) * ((float)j - v0.Y) / (v2.Y - v0.Y);
			int max = (int)v0.X + (v1.X - v0.X) * ((float)j - v0.Y) / (v1.Y - v0.Y);
			for (int i = min; i < max; ++i)
			{
				if(i >= 0 && i < int(Graphics::ScreenWidth) && j >= 0 && j < int(Graphics::ScreenHeight))
					gfx.PutPixel(i, j, c);
			}
				
		}
		for (int j = (int)v1.Y; j < (int)v2.Y; j++)
		{
			int min = (int)v0.X + (v2.X - v0.X) * ((float)j - v0.Y) / (v2.Y - v0.Y);
			int max = (int)v1.X + (v2.X - v1.X) * ((float)j - v1.Y) / (v2.Y - v1.Y);
			for (int i = min; i < max; ++i)
			{
				if (i >= 0 && i < int(Graphics::ScreenWidth) && j >= 0 && j < int(Graphics::ScreenHeight))
					gfx.PutPixel(i, j, c);
			}
		}
	}
	else
	{
		for (int j = (int)v0.Y; j < (int)v1.Y; j++)
		{
			int max = (int)v0.X + (v2.X - v0.X) * ((float)j - v0.Y) / (v2.Y - v0.Y);
			int min = (int)v0.X + (v1.X - v0.X) * ((float)j - v0.Y) / (v1.Y - v0.Y);
			for (int i = min; i < max; ++i)
			{
				if (i >= 0 && i < int(Graphics::ScreenWidth) && j >= 0 && j < int(Graphics::ScreenHeight))
					gfx.PutPixel(i, j, c);
			}
		}
		for (int j = (int)v1.Y; j < (int)v2.Y; j++)
		{
			int max = (int)v0.X + (v2.X - v0.X) * ((float)j - v0.Y) / (v2.Y - v0.Y);
			int min = (int)v1.X + (v2.X - v1.X) * ((float)j - v1.Y) / (v2.Y - v1.Y);
			for (int i = min; i < max; ++i)
			{
				if (i >= 0 && i < int(Graphics::ScreenWidth) && j >= 0 && j < int(Graphics::ScreenHeight))
					gfx.PutPixel(i, j, c);
			}
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
