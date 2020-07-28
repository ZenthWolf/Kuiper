#include "Animation.h"

Animation::Animation( int x0, int y0, int width, int height, int framecount,
	                  const Surface& sprite, float holdTime, Color chroma)
	: chroma(chroma), sprite(sprite), holdTime(holdTime)
{
	frame.reserve(framecount);
	for (int i = 0; i < framecount; i++)
	{
		frame.emplace_back( x0 + (i)*width, y0, x0 + (i + 1) * width, y0+height );
	}
}

void Animation::Update(float dt)
{
	if ((frameTime += dt) >= holdTime)
	{
		Advance();
		frameTime -= holdTime;
	}
}

void Animation::Draw(const VecI& pos, Graphics& gfx) const
{
	gfx.DrawSprite(pos.X, pos.Y, frame[iCurFrame], sprite, chroma);
}

void Animation::Draw(const VecI& pos, Graphics& gfx, RectI& clip) const
{
	gfx.DrawSprite(pos.X, pos.Y, frame[iCurFrame], clip, sprite, chroma);
}

void Animation::Advance()
{
	if (++iCurFrame >= frame.size())
	{
		iCurFrame = 0;
	}
}