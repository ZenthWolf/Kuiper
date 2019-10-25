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
	Player({ 400.0f, 550.0f }, 35.0f, 10.0f),
	Ball(200.0f, 550.0f, 1.5f, 3.0f, Colors::Green),
	wall(0.0f, 0.0f, float(Graphics::ScreenWidth - 1), float(Graphics::ScreenHeight - 1)),
	sndPad(L"Sound\\arkpad.wav"), sndBreak(L"Sound\\arkbrick.wav")
{
	Color Cols[5] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Cyan, {148, 0, 211} };
	
	Vec offset = { (float(Graphics::ScreenWidth) - float(Columns) * BrickWidth) / 2.0f, 50.0f };

	for (int x = 0; x < Columns; x++)
	{
		for (int y = 0; y < Rows; y++)
		{
			Vec Start = { float(x) * BrickWidth, float(y) * BrickHeight };
			Start += offset;
			Vec BVec = { float(BrickWidth), float(BrickHeight) };
			Block[x][y]= Brick( RectF(Start, Start + BVec), Cols[y%5]);
		}
	}
	
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

	Player.Move(wnd.kbd, dt);
	Player.CollWall(wall);

	Ball.Move(dt);
	if (Ball.CollWall(wall))
	{
		sndPad.Play();
	}

	if (Player.CollBall(Ball))
	{
		sndPad.Play();
	}

	bool doColl = false;
	float closeColl;
	int whichColl[2];

	for (int i = 0; i < Columns; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			if (Block[i][j].CollCheck(Ball))
			{
				const float newDist = (Ball.GetPos() - Block[i][j].Rect.Cent()).GetLengthSq();
				if (doColl)
				{
					if (newDist < closeColl)
					{
						closeColl = newDist;
						whichColl[0] = i;
						whichColl[1] = j;
					}
				}
				else
				{
					closeColl = newDist;
					whichColl[0] = i;
					whichColl[1] = j;
					doColl = 1;
				}
			}
		}
	}

	if (doColl)
	{
		Block[whichColl[0]][whichColl[1]].Break(Ball);
		sndBreak.Play();
	}
}



void Game::ComposeFrame()
{
	Ball.Draw(gfx);
	Player.Draw(gfx);

	for (auto& row : Block)
	{
		for (auto& b : row)
		{
			b.Draw(gfx);
		}
	}
}
