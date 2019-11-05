#include "Font.h"
#include <cassert>
#undef DrawText

Font::Font(const std::string& filename, Color chroma)
	:fontSheet(filename), glyphyWidth(fontSheet.Width() / nColumns), glyphHeight(fontSheet.Height() / nRows),
	chromakey(chroma)
{
	assert(glyphyWidth * nColumns == fontSheet.Width());
	assert(glyphHeight * nRows == fontSheet.Height());
}

void Font::DrawText(const std::string& text, const VecI& pos, Color fontCol, Graphics& gfx)
{
	auto curPos = pos;
	for (auto c : text)
	{
		if (c == '\n')
		{
			curPos.X = pos.X;
			curPos.Y += glyphHeight;
			continue;
		}
		if (c >= firstChar + 1 && c <= lastChar)
		{
			gfx.DrawSpriteSubs(curPos.X, curPos.Y, fontCol, MapGlyphRect(c), fontSheet, chromakey);
		}
		curPos.X += glyphyWidth;
	}
}

RectI Font::MapGlyphRect(char c) const
{
	assert(c >= firstChar && c <= lastChar);
	const int glyphIndex = c - firstChar;
	const int glyphX = glyphIndex % nColumns;
	const int glyphY = glyphIndex / nColumns;

	return RectI({ glyphX * glyphyWidth, glyphY * glyphHeight }, { glyphX * glyphyWidth + glyphyWidth, glyphY * glyphHeight + glyphHeight });
}
