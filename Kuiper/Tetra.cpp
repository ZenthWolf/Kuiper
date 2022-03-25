#define NOMINMAX // Needed to make min work
#include <algorithm>
#include "Tetra.h"
#include "Engine/Vec.h"



void Tetrahedron::Update(float dt)
{
	rotX += dt * .45;
	rotZ1 += dt * .7;

	if (rotX > 6.2831853)
		rotX -= 6.2831853;
	else if (rotX < -6.2831853)
		rotX += 6.2831853;

	if (rotZ1 > 6.2831853)
		rotZ1 -= 6.2831853;
	else if (rotZ1 < -6.2831853)
		rotZ1 += 6.2831853;

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
		point[i] = Vec3<float>(vert[i].X * screenDist * invDepth + (float)(cambox.width()) / 2, vert[i].Y * screenDist * invDepth + (float)(cambox.height()) / 2, invDepth);
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
			auto CheckEdge = [&](Vec3<float> v)
			{
				if ((int)v.Y > 0 && (int)v.Y < Graphics::ScreenHeight && (int)v.X > 0 && (int)v.X < Graphics::ScreenWidth)
					return (v.Z + 0.01f) > zBuff[(int)v.Y * Graphics::ScreenWidth + (int)v.X];
				else
					return true; // sure, w/e, I don't care. Look at me commenting how little I care about this lazy handling
			};
			switch (i)
			{
			case 0:
			{
				Color c = { 255,0,0 };
				Color shade = Color(c.GetR() * brightness, c.GetG() * brightness, c.GetB() * brightness);
				DrawTriFromLowest(point[0], point[1], point[2], 0, zBuff, shade, gfx);

				if (CheckEdge(point[0]) && CheckEdge(point[1]) && CheckEdge(point[2]))
				{
					if (edgebrightness[0] < brightness)
					{
						edgebrightness[0] = brightness;
						gfx.DrawLine(point[0].Project(), point[1].Project(), line);
					}
					if (edgebrightness[1] < brightness)
					{
						edgebrightness[1] = brightness;
						gfx.DrawLine(point[0].Project(), point[2].Project(), line);
					}
					if (edgebrightness[3] < brightness)
					{
						edgebrightness[3] = brightness;
						gfx.DrawLine(point[1].Project(), point[2].Project(), line);
					}
				}
				break;
			}
			case 1:
			{
				Color c = { 0,255,0 };
				Color shade = Color(c.GetR() * brightness, c.GetG() * brightness, c.GetB() * brightness);
				DrawTriFromLowest(point[0], point[1], point[3], 1, zBuff, shade, gfx);

				if (CheckEdge(point[0]) && CheckEdge(point[1]) && CheckEdge(point[3]))
				{
					if (edgebrightness[2] < brightness)
					{
						edgebrightness[2] = brightness;
						gfx.DrawLine(point[0].Project(), point[3].Project(), line);
					}
					if (edgebrightness[0] < brightness)
					{
						edgebrightness[0] = brightness;
						gfx.DrawLine(point[0].Project(), point[1].Project(), line);
					}
					if (edgebrightness[4] < brightness)
					{
						edgebrightness[4] = brightness;
						gfx.DrawLine(point[3].Project(), point[1].Project(), line);
					}
				}
				break;
			}
			case 2:
			{
				Color c = { 0,0,255 };
				Color shade = Color(c.GetR() * brightness, c.GetG() * brightness, c.GetB() * brightness);
				DrawTriFromLowest(point[0], point[3], point[2], 2, zBuff, shade, gfx);

				if (CheckEdge(point[0]) && CheckEdge(point[3]) && CheckEdge(point[2]))
				{
					if (edgebrightness[1] < brightness)
					{
						edgebrightness[1] = brightness;
						gfx.DrawLine(point[0].Project(), point[2].Project(), line);
					}
					if (edgebrightness[2] < brightness)
					{
						edgebrightness[2] = brightness;
						gfx.DrawLine(point[0].Project(), point[3].Project(), line);
					}
					if (edgebrightness[5] < brightness)
					{
						edgebrightness[5] = brightness;
						gfx.DrawLine(point[2].Project(), point[3].Project(), line);
					}
				}
				break;
			}
			case 3:
			{
				DrawTriFromLowest(point[1], point[2], point[3], 3, zBuff, shade, gfx);
				
				if (CheckEdge(point[3]) && CheckEdge(point[1]) && CheckEdge(point[2]))
				{
					if (edgebrightness[4] < brightness)
					{
						edgebrightness[4] = brightness;
						gfx.DrawLine(point[3].Project(), point[1].Project(), line);
					}
					if (edgebrightness[5] < brightness)
					{
						edgebrightness[5] = brightness;
						gfx.DrawLine(point[2].Project(), point[3].Project(), line);
					}
					if (edgebrightness[3] < brightness)
					{
						edgebrightness[3] = brightness;
						gfx.DrawLine(point[1].Project(), point[2].Project(), line);
					}
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

void  Tetrahedron::DrawTriFromLowest(Vec3<float>& v0, Vec3<float>& v1, Vec3<float>& v2, int n, float*& zbuff, Color c, Graphics& gfx)
{
	if (v0.Y < v1.Y)
	{
		if (v0.Y < v2.Y)
		{
			if(v1.Y < v2.Y)
				DrawTri(v0, v1, v2, n, zbuff, c, gfx);
			else
				DrawTri(v0, v2, v1, n, zbuff, c, gfx);
			return;
		}
		DrawTri(v2, v0, v1, n, zbuff, c, gfx);
		return;
	}
	else if (v1.Y < v2.Y)
	{
		if(v0.Y < v2.Y)
			DrawTri(v1, v0, v2, n, zbuff, c, gfx);
		else
			DrawTri(v1, v2, v0, n, zbuff, c, gfx);
		return;
	}
	DrawTri(v2, v1, v0, n, zbuff, c, gfx);
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

void Tetrahedron::RotZ1(const float dth)
{
	rotZ1 += dth;

	if (rotZ1 > 6.2831853)
		rotZ1 -= 6.2831853;
	else if (rotZ1 < -6.2831853)
		rotZ1 += 6.2831853;
}

void Tetrahedron::RotZ2(const float dth)
{
	rotZ2 += dth;

	if (rotZ2 > 6.2831853)
		rotZ2 -= 6.2831853;
	else if (rotZ2 < -6.2831853)
		rotZ2 += 6.2831853;
}

Vec3<float> Tetrahedron::GetNorm(const int i)
{
	return norm[i];
}

void Tetrahedron::DrawTri(Vec3<float>&v0, Vec3<float>&v1, Vec3<float>&v2, int n, float*& zBuff, Color c, Graphics & gfx)
{
	//Different approach for CCW and CW orientations.
	float Area = 1.0f/(v1 - v0).Cross(v2 - v0).Z;
	if (Area > 0)
	{
		float miny = v0.Y + 1 > 0 ? v0.Y + 1 : 0;
		float maxy = v1.Y < Graphics::ScreenHeight ? v1.Y : Graphics::ScreenHeight;

		for (float j = miny; j < maxy; j+=1.0f)
		{
			float minx = std::max(0.0f,(v0.X + 1 + (v2.X - v0.X) * (j - v0.Y) / (v2.Y - v0.Y)));
			float maxx = std::min((float)Graphics::ScreenWidth, (v0.X + 1 + (v1.X - v0.X) * (j - v0.Y) / (v1.Y - v0.Y)));

			for (float i = minx; i < maxx; i+=1.0f)
			{
				float invDepth = v0.Z * (1 - abs(Area * (v0.Project() - Vec<float>(i, j)).Cross((v1.Project() - v2.Project())))) +
								v1.Z * (1 - abs(Area * (v1.Project() - Vec<float>(i, j)).Cross((v2.Project() - v0.Project())))) +
								v2.Z * (1 - abs(Area * (v2.Project() - Vec<float>(i, j)).Cross((v0.Project() - v1.Project()))));
 				
				
				if (zBuff[(int)j * Graphics::ScreenWidth + (int)i] < invDepth)
				{
					zBuff[(int)j * Graphics::ScreenWidth + (int)i] = invDepth;
					gfx.PutPixel((int)i, (int)j, c);
				}
				else
				{
					int err = (int)j * Graphics::ScreenWidth + (int)i;
					bool stop = true;
				}
			}
		}

		float j = v1.Y;
		if (j >= 0 && j < float(Graphics::ScreenHeight) && (int)v1.Y != (int)v0.Y)
		{
			float minx = std::max(0.0f, (v0.X + 1 + (v2.X - v0.X) * (j - v0.Y) / (v2.Y - v0.Y)));
			float maxx = std::min((float)Graphics::ScreenWidth,v1.X + 1);
			
			for (float i = minx; i < maxx; i += 1.0f)
			{
				float invDepth = v0.Z * (1 - abs(Area * (v0.Project() - Vec<float>(i, j)).Cross((v1.Project() - v2.Project())))) +
								v1.Z * (1 - abs(Area * (v1.Project() - Vec<float>(i, j)).Cross((v2.Project() - v0.Project())))) +
								v2.Z * (1 - abs(Area * (v2.Project() - Vec<float>(i, j)).Cross((v0.Project() - v1.Project()))));
				if (zBuff[(int)j * Graphics::ScreenWidth + (int)i] < invDepth)
				{
					zBuff[(int)j * Graphics::ScreenWidth + (int)i] = invDepth;
					gfx.PutPixel((int)i, (int)j, c);
				}
				else
				{
					int err = (int)j * Graphics::ScreenWidth + (int)i;
					bool stop = true;
				}
			}
		}

		miny = v1.Y + 1 > 0 ? v1.Y + 1 : 0;
		maxy = v2.Y < (float)Graphics::ScreenHeight ? v2.Y : (float)Graphics::ScreenHeight;
		for (float j = miny; j < maxy; j+=1.0f)
		{
			float minx = std::max(0.0f, (v0.X + 1 + (v2.X - v0.X) * (j - v0.Y) / (v2.Y - v0.Y)));
			float maxx = std::min((float)Graphics::ScreenWidth, (v1.X + 1 + (v2.X - v1.X) * (j - v1.Y) / (v2.Y - v1.Y)));

			for (float i = minx; i < maxx; i += 1.0f)
			{
				float invDepth = v0.Z * (1 - abs(Area * (v0.Project() - Vec<float>(i, j)).Cross((v1.Project() - v2.Project())))) +
								v1.Z * (1 - abs(Area * (v1.Project() - Vec<float>(i, j)).Cross((v2.Project() - v0.Project())))) +
								v2.Z * (1 - abs(Area * (v2.Project() - Vec<float>(i, j)).Cross((v0.Project() - v1.Project()))));
				if (zBuff[(int)j * Graphics::ScreenWidth + (int)i] < invDepth)
				{
					zBuff[(int)j * Graphics::ScreenWidth + (int)i] = invDepth;
					gfx.PutPixel((int)i, (int)j, c);
				}
				else
				{
					int err = (int)j * Graphics::ScreenWidth + (int)i;
					bool stop = true;
				}
			}
		}
	}
	else
	{
		Area *= -1;
		float miny = v0.Y + 1 > 0 ? v0.Y + 1 : 0;
		float maxy = v1.Y < (float)Graphics::ScreenHeight ? v1.Y : (float)Graphics::ScreenHeight;
		for (float j = miny; j < maxy; j+=1.0f)
		{
			float maxx = std::min((float)Graphics::ScreenWidth, (v0.X + 1 + (v2.X - v0.X) * (j - v0.Y) / (v2.Y - v0.Y)));
			float minx = std::max(0.0f, (v0.X + 1 + (v1.X - v0.X) * (j - v0.Y) / (v1.Y - v0.Y)));
			
			for (float i = minx; i < maxx; i += 1.0f)
			{
				float invDepth = v0.Z * (1 - abs(Area * (v0.Project() - Vec<float>(i, j)).Cross((v1.Project() - v2.Project())))) +
								v1.Z * (1 - abs(Area * (v1.Project() - Vec<float>(i, j)).Cross((v2.Project() - v0.Project())))) +
								v2.Z * (1 - abs(Area * (v2.Project() - Vec<float>(i, j)).Cross((v0.Project() - v1.Project()))));
				if (zBuff[(int)j * Graphics::ScreenWidth + (int)i] < invDepth)
				{
					zBuff[(int)j * Graphics::ScreenWidth + (int)i] = invDepth;
					gfx.PutPixel((int)i, (int)j, c);
				}
				else
				{
					int err = (int)j * Graphics::ScreenWidth + (int)i;
					bool stop = true;
				}
			}
		}

		float j = v1.Y;
		if (j >= 0 && j < float(Graphics::ScreenHeight) && (int)v1.Y != (int)v0.Y)
		{
			float maxx = std::min((float) Graphics::ScreenWidth, (v0.X + 1 + (v2.X - v0.X) * (j - v0.Y) / (v2.Y - v0.Y)));
			float minx = std::max(0.0f, v1.X + 1);

			for (float i = minx; i < maxx; i += 1.0f)
			{
				float invDepth = v0.Z * (1 - abs(Area * (v0.Project() - Vec<float>(i, j)).Cross((v1.Project() - v2.Project())))) +
								v1.Z * (1 - abs(Area * (v1.Project() - Vec<float>(i, j)).Cross((v2.Project() - v0.Project())))) +
								v2.Z * (1 - abs(Area * (v2.Project() - Vec<float>(i, j)).Cross((v0.Project() - v1.Project()))));
				if (zBuff[(int)j * Graphics::ScreenWidth + (int)i] < invDepth)
				{
					zBuff[(int)j * Graphics::ScreenWidth + (int)i] = invDepth;
					gfx.PutPixel((int)i, (int)j, c);
				}
				else
				{
					int err = (int)j * Graphics::ScreenWidth + (int)i;
					bool stop = true;
				}
			}
		}

		miny = v1.Y + 1 > 0 ? v1.Y + 1 : 0;
		maxy = v2.Y < (float)Graphics::ScreenHeight ? v2.Y : (float)Graphics::ScreenHeight;
		for (float j = miny; j < maxy; j+=1.0f)
		{
			float maxx = std::min((float)Graphics::ScreenWidth, (v0.X + 1 + (v2.X - v0.X) * (j - v0.Y) / (v2.Y - v0.Y)));
			float minx = std::max(0.0f, (v1.X + 1 + (v2.X - v1.X) * (j - v1.Y) / (v2.Y - v1.Y)));
			
			for (float i = minx; i < maxx; i += 1.0f)
			{
				float invDepth = v0.Z * (1 - abs(Area * (v0.Project() - Vec<float>(i, j)).Cross((v1.Project() - v2.Project())))) +
								v1.Z * (1 - abs(Area * (v1.Project() - Vec<float>(i, j)).Cross((v2.Project() - v0.Project())))) +
								v2.Z * (1 - abs(Area * (v2.Project() - Vec<float>(i, j)).Cross((v0.Project() - v1.Project()))));
				if (zBuff[(int)j * Graphics::ScreenWidth + (int)i] < invDepth)
				{
					zBuff[(int)j * Graphics::ScreenWidth + (int)i] = invDepth;
					gfx.PutPixel((int)i, (int)j, c);
				}
				else
				{
					int err = (int)j * Graphics::ScreenWidth + (int)i;
					bool stop = true;
				}
			}
		}
	}
}

void Tetrahedron::TransformModel()
{
	for (int i = 0; i < 4; ++i)
	{
		vert[i] = model[i] * (Matrix::ZRotor(rotZ1) * Matrix::XRotor(rotX) * Matrix::ZRotor(rotZ2));
		vert[i] += pos;
		norm[i] = modnorm[i] * (Matrix::ZRotor(rotZ1) * Matrix::XRotor(rotX) * Matrix::ZRotor(rotZ2));
	}
}
