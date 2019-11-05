#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "VecI.h"
#undef DrawText

class Font
{
public:
	Font(const std::string& filename, Color chroma = Colors::White);
	void DrawText(const std::string& text, const VecI& pos, Color fontCol, Graphics& gfx);

private:
	RectI MapGlyphRect(char c) const;

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