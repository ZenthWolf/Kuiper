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

#include "Colors.h"
#include <string>
#include<vector>
#include "../Engine/Rect.h"
#include <cassert>

class Surface
{
public:
	Surface(const std::string& filename);
	Surface(int width, int height);
	Surface(Surface&& donor);
	Surface(const Surface&) = default;
	Surface& operator =(Surface&& rhs);
	Surface& operator =(const Surface& rhs) = default;
	Surface() = default;
	void PutPixel(int x, int y, Color C);
	Color GetPixel(int x, int y) const;
	int Width() const;
	int Height() const;
	Rect<int> Rect() const;
	void Fill(Color c);
	const Color* Data() const;

private:
	std::vector<Color> pixels;
	int width = 0;
	int height = 0;
};