/* These functions are optimized in compilation for
   better performance while debugging */

#define SPRITE_OPTIMIZE
#include"Graphics.h"

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset(pSysBuffer, 0u, sizeof(Color) * Graphics::ScreenHeight * Graphics::ScreenWidth);
}

void Graphics::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < int(Graphics::ScreenWidth));
	assert(y >= 0);
	assert(y < int(Graphics::ScreenHeight));
	if (x > 0 && x < ScreenWidth && y>0 && y < ScreenHeight)
	{
		pSysBuffer[Graphics::ScreenWidth * y + x] = c;
	}
}

Color Graphics::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < int(Graphics::ScreenWidth));
	assert(y >= 0);
	assert(y < int(Graphics::ScreenHeight));

	return pSysBuffer[Graphics::ScreenWidth * y + x];
}

//Optimized Templates
void dummy(Graphics& gfx)
{
	gfx.DrawSprite(0, 0, Rect<int>{ 0,0,0,0 }, Rect<int>{ 0,0,0,0 }, Surface{}, SpriteEffect::Copy{});
	gfx.DrawSprite(0, 0, Rect<int>{ 0,0,0,0 }, Rect<int>{ 0,0,0,0 }, Surface{}, SpriteEffect::Chroma{ Colors::Black });
	gfx.DrawSprite(0, 0, Rect<int>{ 0,0,0,0 }, Rect<int>{ 0,0,0,0 }, Surface{}, SpriteEffect::Substitute{ Colors::Black,Colors::Black });
}
