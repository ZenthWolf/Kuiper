/***************************************************************************/
/***               Temper Tech DirectX Framework V 1.0.0                 ***/
/*** Game.cpp                                                            ***/
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

#include "MainWindow.h"
#include "Game.h"


Game::Game(MainWindow& wnd)
	:
	wnd(wnd), gfx(wnd),
	Player(gfx, wnd.kbd, 400.0f, 550.0f, 75.0f, 15.0f, Colors::Gray),
	Ball(gfx, 200, 500, 1.0f, 1.5f, Colors::Green)
{
}

void Game::Play()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	float dt = ft.Mark();

	Player.Update(wnd.kbd, dt);

	Ball.Update(dt);
}


void Game::ComposeFrame()
{
	Ball.Draw();
	Player.Draw();


	/*** Test Rects for Wall Behavior ***/
	float t = 5.0f;
	float SH = float(gfx.ScreenHeight);
	float SW = float(gfx.ScreenWidth);

	RectF Left(gfx, 0.0f, 0.0f, t, SH -1.0f, Colors::Red);
	RectF Right(gfx, SW - t - 1.0f, 0.0f, t, SH - 1.0f, Colors::Red);
	RectF Top(gfx, 0.0f, 0.0f, SW-1.0f, t, Colors::Red);
	RectF Bot(gfx, 0.0f, SH - t - 1.0f, SW-1.0f, t, Colors::Red);

	Left.Draw();
	Right.Draw();
	Top.Draw();
	Bot.Draw();
}
