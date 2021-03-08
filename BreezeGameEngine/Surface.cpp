#include "Surface.h"
#include "WinDefs.h"
#include <cassert>
#include <fstream>
#include <math.h>

Surface::Surface(const std::string& filename)
{

	/* Uncompressed RGB bitmap */
	std::ifstream sprite(filename, std::ios::binary);
	assert(sprite);

	BITMAPFILEHEADER spriteHeader;
	sprite.read( reinterpret_cast<char*>(&spriteHeader) , sizeof(spriteHeader) );
	BITMAPINFOHEADER spriteInfo;
	sprite.read(reinterpret_cast<char*>(&spriteInfo), sizeof(spriteInfo));

	assert(spriteInfo.biBitCount == 24 || spriteInfo.biBitCount == 32);
	assert(spriteInfo.biCompression == BI_RGB);
	
	width = spriteInfo.biWidth;
	height = abs(spriteInfo.biHeight);
	pixels.resize(width * height);

	sprite.seekg(spriteHeader.bfOffBits);
	/* Pixels aren't padded, but rows are- each row is a multiple of 4 bytes */

	const int padding = (4 - (width * 3) % 4) % 4;

	/* first bitmap points are image BOTTOM */
	int imagesgn = spriteInfo.biHeight / height;
	int imagetop = (height + imagesgn * height - 1) / 2;
	int imagebot = (height - imagesgn * height - 1) / 2;
	int bitcount = spriteInfo.biBitCount;
	for (int y =  imagetop; y != imagebot - imagesgn; y -= imagesgn)
	{
		for (int x = 0; x < width; x++)
		{
			if (bitcount == 24)
			{
				PutPixel(x, y, Color(sprite.get(), sprite.get(), sprite.get()));
			}
			else if (bitcount == 32)
			{
				PutPixel(x, y, Color(sprite.get(), sprite.get(), sprite.get(), sprite.get()));
			}
		}
		if (bitcount == 24)
		{
			sprite.seekg(padding, std::ios::cur);
		}
	}
}

Surface::Surface(int width, int height)
	:width(width), height(height), pixels(width*height)
{
}

Surface::Surface(Surface&& donor)
{
	*this = std::move(donor);
}

Surface& Surface::operator=( Surface&& rhs)
{
	width = rhs.Width();
	height = rhs.Height();
	pixels = std::move(rhs.pixels);
	rhs.width = 0;
	rhs.height = 0;

	return *this;
}

int Surface::Width() const
{
	return width;
}

int Surface::Height() const
{
	return height;
}

Rect<int> Surface::Rect() const
{
	return {0, 0, width, height};
}

const Color* Surface::Data() const
{
	return pixels.data();
}
