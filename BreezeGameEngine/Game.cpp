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
	ct(gfx), cam(ct), spawner(belt), mus(L"Sound\\ForgetAboutMe.wav", Sound::LoopType::AutoFullSound)
{
	std::uniform_real_distribution<float> xDist(-3000.0f, 3000.0f);
	std::uniform_real_distribution<float> yDist(-3000.0f, 3000.0f);
	std::uniform_real_distribution<float> scaleDist(0.3f, 10.0f);
	std::uniform_real_distribution<float> rotDist(0, 2 * 3.1415926);
	
	for (int i = 0; i < 150; i++)
	{
		scene.emplace_back( Star(Vec<float>{xDist(rng), yDist(rng)}) );
		scene[i].SetScale(scaleDist(rng));
		scene[i].SetHeading(rotDist(rng));
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

			mus.Play(1.0f, 0.3f);
		}
		break;

	case GameState::Play:
		float avel = 3.0f;
		if (wnd.kbd.KeyIsPressed('W'))
		{
			Vec<float> heading = ship.GetHeading();
			ship.Thrust(dt);
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			ship.AThrust(dt);
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			ship.AThrust(-dt);
		}

		ship.Update(dt);
		
		spawner.Update(dt, cam.GetScreenBox());

		if (!wnd.kbd.KeyIsEmpty())
		{
			auto e = wnd.kbd.ReadKey();
			switch (e.GetCode())
			{
			case VK_SHIFT:
				if (e.IsPress())
				{
					collship = !collship;
				}
				break;
			}
		}

		if (collship)
		{
			spawner.CollideShip(ship);
		}


		const float velc = 200.0f;
		if (wnd.kbd.KeyIsPressed(VK_UP))
		{
			cam.MoveBy({ 0.0f, dt * velc });
		}
		if (wnd.kbd.KeyIsPressed(VK_LEFT))
		{
			cam.MoveBy({ -dt * velc, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed(VK_DOWN))
		{
			cam.MoveBy({ 0.0f, -dt * velc });
		}
		if (wnd.kbd.KeyIsPressed(VK_RIGHT))
		{
			cam.MoveBy({ dt * velc, 0.0f });
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

	Rect<float> cambox = cam.GetScreenBox().GetExpand(-150.0f);
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
		std::string title = "    KUIPER\n\nW, A, D -> Move\nArrows -> Pan Camera\n\nStatus: Flying only\n\nSpace -> Start";
		font.DrawText(title, { 280,235 }, Colors::White, gfx);
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

		for (auto& s : scene)
		{
			cam.Draw(s.GetDrawable());
		}

		cam.Draw(Drawable(poly, Colors::Green));

		cam.Draw(ship.GetDrawable());

		for (auto& a : belt)
		{
			cam.Draw(a->GetDrawable());

			std::vector<Vec<float>> test;

			test.emplace_back(a->GetPos() + Vec<float>{1.0f, 0.0f});
			test.emplace_back(a->GetPos() - Vec<float>{1.0f, 0.0f});

			Drawable d(test, Colors::LightBlue);
			cam.Draw(d);
		}

		font.DrawText(std::to_string(belt.size()), {100,100}, Colors::White, gfx);
		if (collship)
		{
			font.DrawText("Colliding", { 50,65 }, Colors::White, gfx);
		}
		font.DrawText("Ship V: " + std::to_string(ship.GetVel().GetLength()), { 50,30 }, Colors::White, gfx);
		if (wnd.mouse.LeftIsPressed())
		{
			std::vector<Vec<float>> testLine;
			Vec<float> pos = { (float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY() };
			testLine.emplace_back(cam.TransformToRealSpace(pos));
			testLine.emplace_back(cam.TransformToRealSpace(Vec<float>{ 100.0f, 100.0f }));

			if (ship.CollPoint(testLine[0]))
			{
				cam.Draw(Drawable(testLine, Colors::Cyan));
			}
			else
			{
				cam.Draw(Drawable(testLine, Colors::Red));
			}
			
		}

		break;
	}
	}
}
