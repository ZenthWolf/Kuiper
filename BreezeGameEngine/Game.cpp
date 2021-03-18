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
	ct(gfx), cam(ct)
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
		if (wnd.kbd.KeyIsPressed('C'))
		{
			if (spawnhalt)
			{
				spawnhalt = false;
			}
		}
		if (wnd.kbd.KeyIsPressed('V'))
		{
			belt.clear();
		}
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			if (!spawnhalt)
			{
				spawnhalt = true;

				Rect<float> bounds = cam.GetScreenBox();

				std::uniform_int_distribution<int> side(0,3);
				std::uniform_real_distribution<float> xDist(bounds.X0, bounds.X1);
				std::uniform_real_distribution<float> yDist(bounds.Y0, bounds.Y1);
				std::uniform_real_distribution<float> vDist(-300.0f, 300.0f);
				std::uniform_real_distribution<float> sDist(0.75f, 5.0f);
				std::uniform_real_distribution<float> thDist(-5.0f, 5.0f);

				int spawnside = side(rng);

				if (spawnside == 0) //TOP
				{
					float scale = sDist(rng);
					Vec<float> pos = { xDist(rng), bounds.Y1 };
					Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
					vel.Y = -abs(vel.Y);
					float rot = thDist(rng) / scale;
					belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
					belt[belt.size() - 1]->SetScale(scale);
				}
				
				if (spawnside == 1) //BOTTOM
				{
					float scale = sDist(rng);
					Vec<float> pos = { xDist(rng), bounds.Y0 };
					Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
					vel.Y = abs(vel.Y);
					float rot = thDist(rng) / scale;
					belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
					belt[belt.size() - 1]->SetScale(scale);
				}

				if (spawnside == 2) //LEFT
				{
					float scale = sDist(rng);
					Vec<float> pos = { bounds.X0, yDist(rng) };
					Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
					vel.X = abs(vel.X);
					float rot = thDist(rng) / scale;
					belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
					belt[belt.size() - 1]->SetScale(scale);
				}

				if (spawnside == 3) //RIGHT
				{
					float scale = sDist(rng);
					Vec<float> pos = { bounds.X1, yDist(rng) };
					Vec<float> vel = { vDist(rng) / scale, vDist(rng) / scale };
					vel.X = -abs(vel.X);
					float rot = thDist(rng) / scale;
					belt.push_back(std::make_unique<Asteroid>(pos, vel, rot));
					belt[belt.size() - 1]->SetScale(scale);
				}
			}
		}

		ship.Update(dt);
		
		for (auto& e : belt)
		{
			e->Update(dt);
		}

		for (int i = 0; i < int(belt.size()); i++)
		{
			for (int j = i + 1; j < belt.size(); j++)
			{
				float dist2 = ( belt[i]->GetPos() - belt[j]->GetPos() ).GetLengthSq();
				float radi2 = belt[i]->GetRadius() + belt[j]->GetRadius();
				radi2 = radi2 * radi2;
				if (radi2 > dist2)
				{
					if ( belt[i]->CollWith(*belt[j]) )
					{
						belt[i]->SetVel(-belt[i]->GetVel());
						belt[j]->SetVel(-belt[j]->GetVel());
					}
				}
			}
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

		cam.Draw(ship.GetDrawable());
		for (auto& e : scene)
		{
			cam.Draw(e.GetDrawable());
		}

		for (auto& e : belt)
		{
			cam.Draw(e->GetDrawable());
		}
		break;
	}
	}
}
