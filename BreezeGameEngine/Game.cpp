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
	Player(gfx, wnd.kbd, 400.0f, 550.0f, 75.0f, 15.0f, Colors::Gray)
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
}


void Game::ComposeFrame()
{
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		5709.0;
	}
	Player.Draw();

}
