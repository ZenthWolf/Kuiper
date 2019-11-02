#include "Surface.h"
#include "WinDefs.h"
#include <cassert>
#include <fstream>
#include <math.h>

Surface::Surface(std::string filename)
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
	pPixel = new Color[width * height];

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
	:width(width), height(height), pPixel( new Color[width*height] )
{
}

Surface::Surface(const Surface& rhs)
	:width(rhs.Width()), height(rhs.Height()), pPixel( new Color[width*height] )
{
	int nPixel = width * height;
	for (int i = 0; i < nPixel; i++)
	{
		pPixel[i] = rhs.pPixel[i];
	}
}

Surface::~Surface()
{
	delete[] pPixel;
	pPixel = nullptr;
}

Surface& Surface::operator=(const Surface& rhs)
{
	width = rhs.Width();
	height = rhs.Height();

	pPixel = new Color[width * height];

	int nPixel = width * height;
	for (int i = 0; i < nPixel; i++)
	{
		pPixel[i] = rhs.pPixel[i];
	}

	return *this;
}

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);

	pPixel[y * width + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);

	return pPixel[y * width + x];
}

int Surface::Width() const
{
	return width;
}

int Surface::Height() const
{
	return height;
}
