/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#pragma once

#include "Graphics/Colors.h"
#include "Graphics/Graphics.h"

namespace SpriteEffect
{
	class Chroma
	{
	public:
		Chroma(Color c) : chroma(c)
		{}

		void operator()(Color srcPix, int xDest, int yDest, Graphics& gfx) const
		{
			if (srcPix != chroma)
			{
				gfx.PutPixel(xDest, yDest, srcPix);
			}
		}
	private:
		Color chroma = Colors::Magenta;
	};

	class Substitute
	{
	public:
		Substitute(Color c, Color sub) : chroma(c), sub(sub)
		{}

		void operator()(Color srcPix, int xDest, int yDest, Graphics& gfx) const
		{
			if (srcPix != chroma)
			{
				gfx.PutPixel(xDest, yDest, sub);
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};

	class Copy
	{
	public:
		void operator()(Color srcPix, int xDest, int yDest, Graphics& gfx) const
		{
			gfx.PutPixel(xDest, yDest, srcPix);
		}
	};

	class Ghost
	{
	public:
		Ghost(Color c) : chroma(c)
		{}

		void operator()(Color srcPix, int xDest, int yDest, Graphics& gfx) const
		{
			if (srcPix != chroma)
			{
				const Color destPix = gfx.GetPixel(xDest, yDest);
				const Color blend = { unsigned char( (srcPix.GetR() + destPix.GetR()) / 2 ),
									  unsigned char( (srcPix.GetG() + destPix.GetG()) / 2 ),
									  unsigned char( (srcPix.GetB() + destPix.GetB()) / 2 )
				                    };
				gfx.PutPixel(xDest, yDest, blend);
			}
		}
	private:
		Color chroma = Colors::Magenta;
	};
}
