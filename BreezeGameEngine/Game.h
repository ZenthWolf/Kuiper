/***************************************************************************/
/***               Temper Tech DirectX Framework V 1.0.0                 ***/
/*** Game.h                                                              ***/
/*** Copyright for all time (if your law disagrees, it can suck it)      ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech                      ***/
/***                                                                     ***/
/*** If you need help, go fuck yourself.                                 ***/
/***************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include <chrono>
#include "Sound.h"

#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Wall.h"

class Game
{
public:
	Game(class MainWindow& wnd);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Play();
private:
	void ComposeFrame();
	void UpdateModel(float ts);
	/********************************/
	/*  User Functions              */
	/********************************/

private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/
	static constexpr float horiBuffer = 50.0f;
	static constexpr float vertBuffer = 10.0f;
	Wall Wall;
	static constexpr float BrickWidth = 50.0f;
	static constexpr float BrickHeight = 20.0f;
	static constexpr int Columns = 10;
	static constexpr int Rows = 5;
	Brick Block[Columns][Rows];
	FrameTimer ft;
	Paddle Player;
	Ball Ball;
//	RectF wall;

	Sound sndPad;
	Sound sndBreak;
};