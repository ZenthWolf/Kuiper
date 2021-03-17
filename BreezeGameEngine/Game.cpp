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
	ct(gfx), cam(ct), ship(Ship::Make())
{
	ship.SetColor(Colors::Cyan);

	std::vector<Vec<float>> poly;
	poly.reserve(5);
	poly.emplace_back(0.0f, 20.0f);
	poly.emplace_back(-5.0f, 7.0f);
	poly.emplace_back(-20.0f, 10.0f);
	poly.emplace_back(-10.0f, -5.0f);
	poly.emplace_back(-17.0f, -20.0f);
	poly.emplace_back(0.0f, -10.0f);
	poly.emplace_back(17.0f, -20.0f);
	poly.emplace_back(10.0f, -5.0f);
	poly.emplace_back(20.0f, 10.0f);
	poly.emplace_back(5.0f, 7.0f);

	std::uniform_real_distribution<float> xDist(-3000.0f, 3000.0f);
	std::uniform_real_distribution<float> yDist(-3000.0f, 3000.0f);
	std::uniform_real_distribution<float> scaleDist(0.3f, 10.0f);
	
	for (int i = 0; i < 150; i++)
	{
		scene.emplace_back(poly, Vec<float>{xDist(rng), yDist(rng)}, Colors::Yellow);
		scene[i].SetScale(scaleDist(rng));
	}

	ship.SetHeading(3.1415926 / 2);
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
		const float vel = 200.0f;
		float avel = 3.0f;
		if (wnd.kbd.KeyIsPressed('W'))
		{
			float th = ship.GetHeading();
			ship.TranslateBy({ cos(th) * dt * vel, sin(th) * dt * vel });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			ship.RotBy(dt * avel);
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			ship.RotBy(-dt * avel);
		}

		const float velc = 200.0f;
		if (wnd.kbd.KeyIsPressed(VK_UP))
		{
			cam.MoveBy({ 0.0f, dt * vel });
		}
		if (wnd.kbd.KeyIsPressed(VK_LEFT))
		{
			cam.MoveBy({ -dt * vel, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed(VK_DOWN))
		{
			cam.MoveBy({ 0.0f, -dt * vel });
		}
		if (wnd.kbd.KeyIsPressed(VK_RIGHT))
		{
			cam.MoveBy({ dt * vel, 0.0f });
		}

		while (!wnd.mouse.IsEmpty())
		{
			const auto e = wnd.mouse.Read();
			switch (e.GetType())
			{
			case Mouse::Event::Type::WheelUp:
				cam.SetScale(cam.GetScale() * 1.05f);
				break;
			case Mouse::Event::Type::WheelDown:
				cam.SetScale(cam.GetScale() * 0.95f);
				break;
			}
		}
		break;
	}

	Rect<float> cambox = cam.GetScreenBox().GetExpand(-50.0f);
	const float velc = 200.0f;

	if (ship.GetPos().X > cambox.X0 ) 
	{
		cam.MoveBy({ dt * velc, 0.0f });
	}

	if (ship.GetPos().X < cambox.X1 )
	{
		cam.MoveBy({ -dt * velc, 0.0f });
	}

	if (ship.GetPos().Y > cambox.Y0 )
	{
		cam.MoveBy({ 0.0f, dt * velc });
	}

	if (ship.GetPos().Y < cambox.Y1 )
	{
		cam.MoveBy({ 0.0f, -dt * velc });
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
		cam.Draw(Drawable(poly, Colors::Green));

		cam.Draw(ship.GetDrawable());
		for (auto& e : scene)
		{
			cam.Draw(e.GetDrawable());
		}

		if (wnd.mouse.LeftIsPressed())
		{
			Vec<float> pos = { (float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY() };
			gfx.DrawLine(Vec<float>{ 100.0f, 100.0f }, pos, Colors::Cyan);
		}
		break;
	}
	}
}
