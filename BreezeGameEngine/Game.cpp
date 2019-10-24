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
	Player({ 400.0f, 550.0f }, 35.0f, 10.0f),
	Ball(200.0f, 550.0f, 2.0f, 3.0f, Colors::Green),
	wall(0.0f, 0.0f, float(Graphics::ScreenWidth-1),float(Graphics::ScreenHeight-1)),
	sndPad(L"Sound\\arkpad.wav")
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

	Player.Move(wnd.kbd, dt);
	Player.CollWall(wall);

	Ball.Move(dt);
	if (Ball.CollWall(wall))
	{
		Ball.CollWall(wall);
		sndPad.Play();
	}

	Player.CollBall(Ball);
}


void Game::ComposeFrame()
{
	Ball.Draw(gfx);
	Player.Draw(gfx);
}
