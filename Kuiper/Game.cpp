/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#include "Engine/MainWindow.h"
#include "Game.h"


Game::Game(MainWindow& AFakeName)
	:
	wnd(AFakeName),gfx(wnd), rng(std::random_device()()),
	ct(gfx), cam(ct), spawner(belt), collider(belt, ship),
	mus(L"Sound\\ForgetAboutMe.wav", Sound::LoopType::AutoFullSound)
{
	SoundSystem::SetMasterVolume(vol);

	std::uniform_real_distribution<float> xDist(-3000.0f, 3000.0f);
	std::uniform_real_distribution<float> yDist(-3000.0f, 3000.0f);
	std::uniform_real_distribution<float> scaleDist(0.3f, 10.0f);
	std::uniform_real_distribution<float> rotDist(0, 2 * 3.1415926);
	
	for (int i = 0; i < 150; ++i)
	{
		scene.emplace_back( Star(Vec<float>{xDist(rng), yDist(rng)}) );
		scene[i].SetScale(scaleDist(rng));
		scene[i].SetHeading(rotDist(rng));
	}

	ship.SetHeading(3.1415926 / 2);
	ship.SetScale(3.0f);

	spawner.Initialize(cam.GetScreenBox());
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
		if (!wnd.kbd.KeyIsEmpty())
		{
			auto e = wnd.kbd.ReadKey();

			if (e.IsPress())
			{
				switch (e.GetCode())
				{
				case 'E':
					vol += 0.05f;
					if (vol > 1.0f)
					{
						vol = 1.0f;
					}
					SoundSystem::SetMasterVolume(vol);
					break;

				case 'Q':
					vol -= 0.05f;
					if (vol < 0.0f)
					{
						vol = 0.0f;
					}
					SoundSystem::SetMasterVolume(vol);
					break;
				case VK_SPACE:
					gameState = GameState::Play;
					mus.Play(1.0f, 1.0f);
					break;
				}
			}
		}
		break;


	case GameState::Play:
		//Volume Control
		if (!wnd.kbd.KeyIsEmpty())
		{
			auto e = wnd.kbd.ReadKey();
			if (e.IsPress())
			{
				switch (e.GetCode())
				{
				case VK_SHIFT:

					collship = !collship;
					break;
				case 'E':
					vol += 0.05f;
					if (vol > 1.0f)
					{
						vol = 1.0f;
					}
					SoundSystem::SetMasterVolume(vol);
					break;
				case 'Q':
					vol -= 0.05f;
					if (vol < 0.0f)
					{
						vol = 0.0f;
					}
					SoundSystem::SetMasterVolume(vol);
					break;
				}
			}
		}

		//Ship Control
		if (wnd.kbd.KeyIsPressed('W'))
		{
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
		//Debug
		if (wnd.kbd.KeyIsPressed('S'))
		{
			ship.SetVel({ 0.0f,0.0f });
			ship.SetAngVel(0.0f);
		}

		//Camera Control
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

		//Camera Zoom
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

		//Auto Pan
		Rect<float> cambox = cam.GetScreenBox().GetExpand(-150.0f);

		if (ship.GetPos().X > cambox.X0)
		{
			cam.MoveBy({ dt * velc, 0.0f });
		}

		if (ship.GetPos().X < cambox.X1)
		{
			cam.MoveBy({ -dt * velc, 0.0f });
		}

		if (ship.GetPos().Y > cambox.Y0)
		{
			cam.MoveBy({ 0.0f, dt * velc });
		}

		if (ship.GetPos().Y < cambox.Y1)
		{
			cam.MoveBy({ 0.0f, -dt * velc });
		}

		//Update
		ship.SetHistory();
		ship.Update(dt);
		spawner.Update(dt, cam.GetScreenBox());
		collider.DoCollisions(dt, collship, jank);
		//ship.DriftDecay(dt);

		break;
	}
}

void Game::ComposeFrame()
{
	switch (gameState)
	{
	case GameState::Title:
	{
		font.DrawText("Volume: " + std::to_string(int(vol*100)) + "\n Q/E = Up/Down", {28,28}, Colors::White, gfx);

		std::string title = "    KUIPER\n\n\nW, A, D -> Move\nArrows -> Pan Camera\n(Autopans at edge)\n\nSpace -> Start\n\nShift -> Toggle Ship Collision";
		font.DrawText(title, { 220,235 }, Colors::White, gfx);
		break;
	}

	case GameState::Play:
	{
		std::vector<Vec<float>> poly;
		poly.reserve(5);
		poly.emplace_back(300.0f, 100.0f);
		poly.emplace_back(400.0f, 500.0f);
		
		//poly.emplace_back(350.0f, 550.0f);
		poly.emplace_back(350.0f, 450.0f);
		
		//poly.emplace_back(250.0f, 550.0f);
		poly.emplace_back(250.0f, 450.0f);
		
		poly.emplace_back(200.0f, 500.0f);

		for (auto& s : scene)
		{
			//cam.Draw(s.GetDrawable());
			cam.Draw(s.GetDrawable());
		}
		
		cam.Draw(Drawable(poly, Colors::Green));
		cam.Draw(ship.GetDrawable());

		std::vector<Vec<float>> DrawShipCenter;

		DrawShipCenter.emplace_back(ship.GetPos());
		DrawShipCenter.emplace_back(ship.GetPos());

		Drawable dsc(DrawShipCenter, Colors::LightBlue);
		cam.Draw(dsc);

		for (auto& a : belt)
		{
			cam.Draw(a->GetDrawable());

			std::vector<Vec<float>> DrawCenter;

			DrawCenter.emplace_back(a->GetPos());
			DrawCenter.emplace_back(a->GetPos());

			Drawable dac(DrawCenter, Colors::LightBlue);
			cam.Draw(dac);
		}

		/*Debug Approach Test Start*/
		auto mothership = Shapes::ConvexSeparator(poly);

		Approach testApproach = Shapes::FindApproach(ship.GetTransformedPrimitives(), mothership);
		std::vector<Vec<float>> approachLine;
		approachLine.emplace_back(testApproach.point0);
		approachLine.emplace_back(testApproach.point1);
		cam.Draw(Drawable(approachLine, Colors::DarkGrey));

		if (wnd.kbd.KeyIsPressed('P'))
		{
			if (wnd.kbd.KeyIsPressed('I'))
			{
				int youViolatedTheLaw = 1;
				bool STOP = youViolatedTheLaw;
			}
			auto shipPrim = ship.GetTransformedPrimitives();
			
			std::vector<Vec<float>> edgeLine;
			std::vector<Vec<float>> pntLine;

			auto shipVert = ship.GetTransformedModel();
			int nVert;// = shipPrim.size();
			auto prim = shipPrim.begin();
			for (int i = 0; i < testApproach.convex0; ++i)
			{
				++prim;
			}
			nVert = (*prim).size();


			if (testApproach.type0 == Approach::Type::Edge)
			{
				Vec<float> lineDir = (*prim)[(testApproach.index0 + 1) % nVert] - (*prim)[testApproach.index0];

				edgeLine.emplace_back((*prim)[testApproach.index0] - lineDir * 2);
				edgeLine.emplace_back((*prim)[testApproach.index0] + lineDir * 3);
				
				cam.Draw(Drawable(edgeLine, Colors::White));
			}
			else if ((testApproach.type0 == Approach::Type::Vertex))
			{
				edgeLine.emplace_back((*prim)[testApproach.index0]);
				edgeLine.emplace_back((*prim)[(testApproach.index0 + 1) % nVert]);
				
				cam.Draw(Drawable(edgeLine, Colors::LightBlue));
			}

			shipVert = poly;
			nVert = shipVert.size();

			if (testApproach.type1 == Approach::Type::Edge)
			{
				pntLine.emplace_back(mothership[testApproach.convex1][testApproach.index1]);
				pntLine.emplace_back(mothership[testApproach.convex1][(testApproach.index1 + 1) % mothership[testApproach.convex1].size()]);
				cam.Draw(Drawable(pntLine, Colors::White));
			}
			else if ((testApproach.type1 == Approach::Type::Vertex))
			{
				pntLine.emplace_back(mothership[testApproach.convex1][testApproach.index1]);
				pntLine.emplace_back(mothership[testApproach.convex1][(testApproach.index1 + 1) % mothership[testApproach.convex1].size()]);
				cam.Draw(Drawable(pntLine, Colors::LightBlue));
			}
		}
		/*Debug Approach Test End*/

		//Debug Info
		font.DrawText(std::to_string(belt.size()), {100,120}, Colors::White, gfx);
		if (collship)
		{
			font.DrawText("Colliding", { 50,90 }, Colors::White, gfx);
		}
		font.DrawText("Ship V: " + std::to_string(ship.GetVel().GetLength()), { 50,30 }, Colors::White, gfx);
		font.DrawText("Ship R: " + std::to_string(ship.GetAngVel()), { 50,60 }, Colors::White, gfx);
		font.DrawText("Janks: " + std::to_string(jank), { 50,570 }, Colors::White, gfx);
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
