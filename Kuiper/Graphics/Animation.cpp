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

#include "Animation.h"
#include "SpriteEffect.h"

Animation::Animation( int x0, int y0, int width, int height, int framecount,
	                  const Surface& sprite, float holdTime, Color chroma)
	: chromakey(chroma), sprite(sprite), holdTime(holdTime)
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

void Animation::Draw(const Vec<int>& pos, Graphics& gfx, bool drawAll) const
{
	if (!drawAll)
	{
		gfx.DrawSprite(pos.X, pos.Y, frame[iCurFrame], sprite, SpriteEffect::Chroma(chromakey));
	}
	else
	{
		gfx.DrawSprite(pos.X, pos.Y, frame[iCurFrame], sprite, SpriteEffect::Copy());
	}
}

void Animation::Draw(const Vec<int>& pos, Graphics& gfx, Color sub) const
{
	gfx.DrawSprite(pos.X, pos.Y, frame[iCurFrame], sprite, SpriteEffect::Substitute(chromakey, sub));
}

void Animation::Draw(const Vec<int>& pos, Graphics& gfx, Rect<int>& clip) const
{
	gfx.DrawSprite(pos.X, pos.Y, frame[iCurFrame], clip, sprite, SpriteEffect::Chroma(chromakey));
}

void Animation::Advance()
{
	if (++iCurFrame >= frame.size())
	{
		iCurFrame = 0;
	}
}
