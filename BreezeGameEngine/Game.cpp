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
	wnd(wnd),
	gfx(wnd),
	rng(std::random_device()())
{

}

void Game::Play()
{
	gfx.BeginFrame();

	float elapseTime = ft.Mark();
	while (elapseTime > 0.0f)
	{
		float dt = std::min(0.0025f, elapseTime);
		UpdateModel(dt);
		elapseTime -= dt;
	}

	ComposeFrame();
	gfx.EndFrame();
}


void Game::UpdateModel(float dt)
{
	link.Update(dt, ground, wnd.kbd);
}


void Game::ComposeFrame()
{
	gfx.DrawRect(Rect<int>(0, 0, 100, 200), Colors::Cyan);
	gfx.DrawRect(Rect<int>(0, 0, 200, 100), Colors::Cyan);

	if (wnd.kbd.KeyIsPressed(VK_CONTROL))
	{
		link.Draw(gfx, Colors::Red);
	}
	else if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		link.Draw(gfx, true);
	}
	else
	{
		link.Draw(gfx);
	}

	ground.Draw(gfx);

	font.DrawText("It's alone to be dangerous. . . \nTake-a a-dis!", { 100, 175 }, Color(255, 255, 255), gfx);
}
