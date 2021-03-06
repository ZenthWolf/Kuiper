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

#include "Graphics.h"
#include "Surface.h"
#include "../Engine/Rect.h"
#undef DrawText

class Font
{
public:
	Font(const std::string& filename, Color chroma = Colors::White);
	void DrawText(const std::string& text, const Vec<int>& pos, Color fontCol, Graphics& gfx) const;
	int GetHeight() const;
	int GetWidth() const;
	int CharWidth(const char c) const;

private:
	Rect<int> MapGlyphRect(char c) const;

	Surface fontSheet;
	/** Fixed Font width **/
	int glyphyWidth;
	int glyphHeight;
	static constexpr int nColumns = 32;
	static constexpr int nRows = 3;

	Color chromakey;
	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
};