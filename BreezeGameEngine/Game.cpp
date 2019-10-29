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
	wnd(wnd),gfx(wnd), rng(std::random_device()()),
	Field(99)
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
	if (wnd.mouse.LeftIsPressed())
	{
		inputPrimed = true;
	}

	if (wnd.mouse.RightIsPressed())
	{
		susPrimed = true;
	}

	if (inputPrimed && !wnd.mouse.LeftIsPressed())
	{
		Field.RevealTile( Field.MouseToTile({ wnd.mouse.GetPosX(), wnd.mouse.GetPosY() }), rng );
		inputPrimed = false;
	}

	if (susPrimed && !wnd.mouse.RightIsPressed())
	{
		Field.SusTile(Field.MouseToTile({ wnd.mouse.GetPosX(), wnd.mouse.GetPosY() }));
		susPrimed = false;
	}

	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		Field.ResetField();
	}
}


void Game::ComposeFrame()
{
	Field.Draw(gfx);
}
