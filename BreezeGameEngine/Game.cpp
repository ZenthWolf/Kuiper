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
#include <assert.h>

#include "Beveler.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd), gfx(wnd),
	Player({ 400.0f, 550.0f }, 50.0f, 4.0f),
	Wall( RectF(horiBuffer , vertBuffer , float(Graphics::ScreenWidth - 1) - horiBuffer, float(Graphics::ScreenHeight - 1) - vertBuffer), 2.0f, Colors::Blue),
    Ball(Player.GetRect().Cent() - Vec(0.0f, Player.height() + Ball.GetRad()), 10.0f, { 0.5f, -0.866f }, Colors::Green),
	sndPad(L"Sound\\arkpad.wav"), sndBreak(L"Sound\\arkbrick.wav")
{
	assert((Columns - 1) * BrickWidth <= Wall.GetBound().width());
	assert((Rows - 1) * BrickHeight <= Wall.GetBound().height());

	Color Cols[5] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Cyan, {148, 0, 211} };
	
	Vec offset = Vec{ (Wall.GetBound().width() - float(Columns) * BrickWidth) / 2.0f, 50.0f } + Wall.GetOrigin();
	

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
	Player.Move(wnd.kbd, dt);
	Player.CollWall(Wall.GetBound());

	Ball.Move(dt);
	if (Ball.CollWall(Wall.GetBound()))
	{
		Player.Cool();
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
		for (int j = 0; j < Rows; j++)
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
		Player.Cool();
		Block[whichColl[0]][whichColl[1]].Break(Ball);
		sndBreak.Play();
	}

	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		for (int i = 0; i < Columns; i++)
		{
			for (int j = 0; j < Rows; j++)
			{
				Block[i][j].Respawn();
			}
		}
		int stop = 0;
	}
}



void Game::ComposeFrame()
{


	Bev Test = Bev(Color(240, 140, 40));

	Test.DrawBevBrick(RectF(390, 290, 410, 310	), 5, gfx);

	Test.ChangeBaseColor(Colors::LightGray);

	Test.DrawBevBorder(RectF(390, 290, 410, 310), 2, gfx);

//	gfx.PutPixel(390, 290, Colors::Blue);
//	gfx.PutPixel(410, 290, Colors::Blue);
//	gfx.PutPixel(390, 310, Colors::Blue);
//	gfx.PutPixel(410, 310, Colors::Blue);

	Ball.Draw(gfx);
	Player.Draw(gfx);

	for (auto& row : Block)
	{
		for (auto& b : row)
		{
			b.Draw(gfx);
		}
	}

	Wall.Draw(gfx);

}
