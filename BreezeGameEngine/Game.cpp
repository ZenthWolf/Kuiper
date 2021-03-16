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
	ct(gfx)
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
	switch (gameState)
	{
	case GameState::Title:
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			gameState = GameState::Play;
		}
		break;

	case GameState::Play:
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			gameState = GameState::Title;
		}

		break;
	}
}


void Game::ComposeFrame()
{
	switch (gameState)
	{
	case GameState::Title:
	{
		std::string title = "KUIPER";
		font.DrawText(title, { 335,255 }, Colors::White, gfx);
		break;
	}

	case GameState::Play:
	{
		std::vector<Vec<float>> poly;
		poly.reserve(5);
		poly.emplace_back(300.0f, 100.0f);
		poly.emplace_back(200.0f, 500.0f);
		poly.emplace_back(250.0f, 450.0f);
		poly.emplace_back(350.0f, 450.0f);
		poly.emplace_back(400.0f, 500.0f);
		gfx.DrawClosedPolyline(poly, Colors::Green);

		std::vector<Vec<float>> poly2;
		poly2.reserve(5);
		poly2.emplace_back(0.0f, -20.0f);
		poly2.emplace_back(-10.0f, 20.0f);
		poly2.emplace_back(-5.0f, 15.0f);
		poly2.emplace_back(5.0f, 15.0f);
		poly2.emplace_back(10.0f, 20.0f);
		ct.DrawPolylineC(poly2, Colors::LightBlue);

		if (wnd.mouse.LeftIsPressed())
		{
			Vec<float> pos = { (float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY() };
			gfx.DrawLine(Vec<float>{ 100.0f, 100.0f }, pos, Colors::Cyan);
		}
		break;
	}
	}
}
