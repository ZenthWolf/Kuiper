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
	ct(gfx), ship(Ship::Make())
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
		const float vel = 300.0f;
		if (wnd.kbd.KeyIsPressed('W') || wnd.kbd.KeyIsPressed(VK_UP))
		{
			ship.TranslateBy({ 0.0f, dt * vel });
		}
		if (wnd.kbd.KeyIsPressed('A') || wnd.kbd.KeyIsPressed(VK_LEFT))
		{
			ship.TranslateBy({ -dt * vel, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S') || wnd.kbd.KeyIsPressed(VK_DOWN))
		{
			ship.TranslateBy({ 0.0f, -dt * vel });
		}
		if (wnd.kbd.KeyIsPressed('D') || wnd.kbd.KeyIsPressed(VK_RIGHT))
		{
			ship.TranslateBy({ dt * vel, 0.0f });
		}

		while (!wnd.mouse.IsEmpty())
		{
			const auto e = wnd.mouse.Read();
			switch (e.GetType())
			{
			case Mouse::Event::Type::WheelUp:
				ship.SetScale(ship.GetScale() * 1.05f);
				break;
			case Mouse::Event::Type::WheelDown:
				ship.SetScale(ship.GetScale() * 0.95f);
				break;
			}
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

		ct.DrawPolylineC(ship.GetPolyline(), Colors::LightBlue);

		if (wnd.mouse.LeftIsPressed())
		{
			Vec<float> pos = { (float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY() };
			gfx.DrawLine(Vec<float>{ 100.0f, 100.0f }, pos, Colors::Cyan);
		}
		break;
	}
	}
}
