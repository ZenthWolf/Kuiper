#include "Font.h"
#include <cassert>
#include "SpriteEffect.h"
#undef DrawText

Font::Font(const std::string& filename, Color chroma)
	:fontSheet(filename), glyphyWidth(fontSheet.Width() / nColumns), glyphHeight(fontSheet.Height() / nRows),
	chromakey(chroma)
{
	assert(glyphyWidth * nColumns == fontSheet.Width());
	assert(glyphHeight * nRows == fontSheet.Height());
}

void Font::DrawText(const std::string& text, const Vec<int>& pos, Color fontCol, Graphics& gfx) const
{
	/** Create functor object **/
	SpriteEffect::Substitute fx(chromakey, fontCol);

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
			gfx.DrawSprite(curPos.X, curPos.Y, MapGlyphRect(c), fontSheet, fx);
		}
		curPos.X += glyphyWidth;
	}
}

int Font::GetHeight() const
{
	return glyphHeight;
}

int Font::GetWidth() const
{
	return glyphyWidth;
}

int Font::CharWidth(const char c) const
{
	return glyphyWidth;
}

Rect<int> Font::MapGlyphRect(char c) const
{
	assert(c >= firstChar && c <= lastChar);
	const int glyphIndex = c - firstChar;
	const int glyphX = glyphIndex % nColumns;
	const int glyphY = glyphIndex / nColumns;

	return Rect<int>({ glyphX * glyphyWidth, glyphY * glyphHeight }, { glyphX * glyphyWidth + glyphyWidth, glyphY * glyphHeight + glyphHeight });
}
