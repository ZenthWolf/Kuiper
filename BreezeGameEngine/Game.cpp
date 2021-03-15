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
	wnd(wnd),gfx(wnd), rng(std::random_device()())
{
	Field = nullptr;
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
		if (wnd.kbd.KeyIsPressed('Q'))
		{
			Field = new MineField(MineField::Difficulty::Beginner);
			gameState = GameState::Play;
		}

		if (wnd.kbd.KeyIsPressed('W'))
		{
			Field = new MineField(MineField::Difficulty::Intermediate);
			gameState = GameState::Play;
		}

		if (wnd.kbd.KeyIsPressed('E'))
		{
			Field = new MineField(MineField::Difficulty::Expert);
			gameState = GameState::Play;
		}

		break;

	case GameState::Play:
	{
		if (Field->CheckD(dt) && !wnd.mouse.IsEmpty())
		{
			//Forces Relief of Double Click bool
			Field->CheckD(0.5f);

			const Mouse::Event e = wnd.mouse.Read();

			switch (e.GetType())
			{
			case Mouse::Event::Type::LPress:
				Field->RevealAdjacent(Field->MouseToTile({ wnd.mouse.GetPosX(), wnd.mouse.GetPosY() }), rng);
				break;
			}
		}

		while (!wnd.mouse.IsEmpty())
		{
			const Mouse::Event e = wnd.mouse.Read();

			switch (e.GetType())
			{
			case Mouse::Event::Type::LPress:
				Field->PrimeL();
				break;

			case Mouse::Event::Type::RPress:
				Field->PrimeR();
				break;

			case Mouse::Event::Type::LRelease:
			{
				Vec<int> mpos = { wnd.mouse.GetPosX(), wnd.mouse.GetPosY() };
				if (Field->CheckL())
				{
					Field->RevealTile(Field->MouseToTile(mpos), rng);

					Field->ReliefL();
				}

				Rect<int> qbox = Rect<int>({ 720,570 }, { 720 + font.GetWidth() * 4, 570 + font.GetHeight() });

				if (qbox.CollWith(mpos))
				{
					gameState = GameState::Title;
					delete Field;
				}

				break;
			}

			case Mouse::Event::Type::RRelease:
				if (Field->CheckR())
				{
					Field->SusTile(Field->MouseToTile({ wnd.mouse.GetPosX(), wnd.mouse.GetPosY() }));

					Field->ReliefR();
				}
				break;
			}
		}

		if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		{
			gameState = GameState::Title;
			delete Field;
		}

		break;
	}
	}
}


void Game::ComposeFrame()
{
	switch (gameState)
	{
	case GameState::Title:
	{
		gfx.DrawRect(0, 0, Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1, Colors::LightGray);
		gfx.DrawCirc({ Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2 }, Graphics::ScreenHeight / 3, Colors::Black);
		std::string menu = "Easy: Q\nMedium: W\nHard: E";
		font.DrawText(menu, { 335,255 }, Colors::White, gfx);
		break;
	}

	case GameState::Play:
		Field->Draw(gfx);
		Vec<int> Claims = Field->GetClaims();

		std::string prog = "Mines: ";
		prog += std::to_string(Claims.X) + "/" + std::to_string(Claims.Y);
		font.DrawText(prog, {20,25}, Colors::White, gfx);

		if (Field->Scored)
		{
			Vec<int> Score = Field->GetScore();

			std::string prog = "Score: ";
			prog += std::to_string(Score.X) + "/" + std::to_string(Score.Y);
			font.DrawText(prog, { 600,25 }, Colors::White, gfx);
		}

		font.DrawText("Quit", { 720,570 }, Colors::White, gfx);

		break;
	}
}
