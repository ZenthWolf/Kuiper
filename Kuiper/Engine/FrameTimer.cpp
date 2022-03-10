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

#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
{
	Last = steady_clock::now();
}

float FrameTimer::Mark()
{
	const auto Old = Last;
	Last = steady_clock::now();
	const duration<float> frameTime = Last - Old;
	return frameTime.count();
}