#include "MineField.h"
#include <assert.h>

MineField::MineField( Difficulty level )
	:LPrimed(false), RPrimed(false), DClickPrimed(false), DClickTimer(0.0f)
{
    TileSize = { 25, 25 };

	switch (level)
	{
	case Difficulty::Beginner:
	{
		Rows = 9;
		Columns = 9;
		Mines = 10;
		break;
	}

	case Difficulty::Intermediate:
	{
		Rows = 16;
		Columns = 16;
		Mines = 40;
		break;
	}

	case Difficulty::Expert:
	{
		Rows = 16;
		Columns = 30;
		Mines = 99;
		break;
	}
	}

	SetField(Rows, Columns, Mines);
	Claims = 0;
	Score = 0;
	Scored = false;
}

MineField::~MineField()
{
	ClearField();
}

void MineField::SetField(int rows, int cols, int mines)
{
	FreeMove = true;
	Rows = rows;
	Columns = cols;
	Mines = mines;

	tile = new Tile[Rows * Columns];

	FieldPos = (Vec<int>({ Graphics::ScreenWidth, Graphics::ScreenHeight }) - Vec<int>({ Columns, Rows }) * TileSize.X) / 2;
}

void MineField::ClearField()
{
	delete [] tile;
	tile = nullptr;
}

void MineField::PrimeL()
{
	LPrimed = true;
}

void MineField::PrimeR()
{
	RPrimed = true;
}

bool MineField::CheckL() const
{
	return LPrimed;
}

bool MineField::CheckR() const
{
	return RPrimed;
}

bool MineField::CheckD(float dt)
{
	if (DClickPrimed)
	{
		DClickTimer += dt;
		//Timer out double click search after 500 ms
		if (DClickTimer >= 0.5f)
		{
			DClickPrimed = false;
		}
	}
	return DClickPrimed;
}

void MineField::ReliefL()
{
	LPrimed = false;
}

void MineField::ReliefR()
{
	RPrimed = false;
}

void MineField::Draw(Graphics& gfx)
{
	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
		{
			Vec<int> loc = Vec<int>{ i * TileSize.X, j * TileSize.Y } + FieldPos;

			if (!tile[j * Columns + i].IsRevealed())
			{

				Bev.ChangeBaseColor(CoveredColor);

				Bev.DrawBevBrick(Rect<int>{ loc, loc + TileSize }.GetExpand(-1), 2, gfx);

				switch (tile[j * Columns + i].IsSus())
				{
				case Tile::Suspicion::Mine:
				{
					gfx.DrawCirc(loc + TileSize / 2 + Vec<int>{ 1,1 }, TileSize.X / 2 - 1, BombColor);

					int hash = 1;
					while (hash <= TileSize.Y - 1)
					{
						for (int x = loc.X; x <= (loc + TileSize).X; x++)
						{
							gfx.PutPixel(x, loc.Y + hash, CoveredColor);
						}
						hash += 3;
					}

					break;
				}

				case Tile::Suspicion::Unsure:
				{
					int hash = 0;
					while (hash <= TileSize.Y)
					{
						for (int x = loc.X; x <= (loc + TileSize).X; x++)
						{
							gfx.PutPixel(x, loc.Y + hash, Colors::Black);
						}
						hash += 3;
					}

					break;
				}
				}
			}
			else
			{
				switch (tile[j * Columns + i].hasContents())
				{
				case Tile::Contents::Empty:
				{
					gfx.DrawRect(Rect<int>{ loc, loc + TileSize }.GetExpand(-1), NumColor[tile[j * Columns + i].GetAdj()]);

					break;
				}

				case Tile::Contents::Mine:

					gfx.DrawRect(Rect<int>{ loc, loc + TileSize }.GetExpand(-1), NumColor[0]);
					gfx.DrawCirc(loc + TileSize / 2 + Vec<int>{1,1}, TileSize.X / 2 - 1, BombColor);
					break;
				}
			}
		}
	}

	Bev.DrawBevBorder(Rect<int>({ (Graphics::ScreenWidth - Columns * TileSize.X) / 2, (Graphics::ScreenHeight - Rows * TileSize.Y) / 2 },
		                    { (Graphics::ScreenWidth + Columns * TileSize.X) / 2, (Graphics::ScreenHeight + Rows * TileSize.Y) / 2 }), 3, gfx);
}

void MineField::RevealTile(const Vec<int> tpos, std::mt19937& rng)
{
	Tile& targ = tile[tpos.Y * Columns + tpos.X];

	if (tpos.X < Columns && tpos.Y < Rows && !targ.IsRevealed() && targ.IsSus() != Tile::Suspicion::Mine)
	{
		targ.OpenTile();

		if (FreeMove)
		{
			assert(Mines < Rows * Columns);

			FreeMove = false;
			PlaceMines(Mines, rng);
		}

		if (targ.hasContents() == Tile::Contents::Empty && targ.GetAdj() == 0)
		{
			for (int i = max(tpos.X - 1, 0); i <= min((tpos.X + 1), Columns-1); i++)
			{
				for (int j = max(tpos.Y - 1, 0); j <= min((tpos.Y + 1), Rows-1); j++)
				{
					RevealTile({ i, j }, rng);
				}
			}
		}

		if (targ.hasContents() == Tile::Contents::Mine && !Scored)
		{
			for (int i = 0; i < Columns * Rows; i++)
			{
				if (tile[i].IsSus() == Tile::Suspicion::Mine && tile[i].hasContents() == Tile::Contents::Mine)
				{
					Score++;
				}
			}
			Scored = true;
		}
	}

	else if (tpos.X < Columns && tpos.Y < Rows && targ.IsRevealed() && targ.GetAdj() > 0 && targ.GetAdj() == AdjMarked(tpos))
	{
		DClickPrimed = true;
		DClickTimer = 0.0f;
	}
}

void MineField::RevealAdjacent(const Vec<int> tpos, std::mt19937& rng)
{
	Tile& targ = tile[tpos.Y * Columns + tpos.X];

	for (int i = max(tpos.X - 1, 0); i <= min((tpos.X + 1), Columns - 1); i++)
	{
		for (int j = max(tpos.Y - 1, 0); j <= min((tpos.Y + 1), Rows - 1); j++)
		{
			if (tile[j * Columns + i].IsSus() != MineField::Tile::Suspicion::Mine)
			{
				RevealTile({ i, j }, rng);
			}

		}
	}
}

void MineField::SusTile(const Vec<int> tpos)
{
	if (tpos.X < Columns && tpos.Y < Rows && !tile[tpos.Y * Columns + tpos.X].IsRevealed())
	{
		tile[tpos.Y * Columns + tpos.X].CycleSus();
		
		switch (tile[tpos.Y * Columns + tpos.X].IsSus())
		{
		case Tile::Suspicion::Mine:
		{
			Claims++;
			break;
		}
		case Tile::Suspicion::Unsure:
		{
			Claims--;
			break;
		}
		}
	}
}

bool MineField::ValidMine(int targ)
{
	int XTarg = targ % Columns;
	int YTarg = (targ - XTarg) / Columns;

	if (tile[targ].hasContents() == Tile::Contents::Empty)
	{
		for (int x = max(XTarg - 1, 0); x <= min(XTarg + 1, Columns - 1); x++)
		{
			for (int y = max(YTarg - 1, 0); y <= min(YTarg + 1, Rows - 1); y++)
			{
				if (tile[y*Columns+x].IsRevealed())
				{
					return false;
				}
			}
		}

		return true;
	}
	else
	{
		return false;
	}

	if (targ > Columns&& targ < Rows * Columns - Columns)
	{
		return tile[targ].hasContents() == Tile::Contents::Empty && !tile[targ].IsRevealed()
			&& !tile[targ+Columns].IsRevealed() && !tile[targ-Columns].IsRevealed()
			&& !tile[targ-1].IsRevealed() && !tile[targ+1].IsRevealed();
	}
	else
	{
		return tile[targ].hasContents() == Tile::Contents::Empty;
	}
}

void MineField::PlaceMines(int mines, std::mt19937& rng)
{
	std::uniform_int_distribution<int> RandTile(0, Rows * Columns - 1);

	while (mines > 0)
	{
		int targ = RandTile(rng);
		if (ValidMine(targ))
		{
			tile[targ].PlaceBomb();
			mines--;
		}
	}

	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
		{
			if (tile[j * Columns + i].hasContents() == Tile::Contents::Empty)
			{
				int adj = 0;
				for (int x = max(i - 1, 0); x <= min(i + 1, Columns-1); x++)
				{
					for (int y = max(j - 1, 0); y <= min(j + 1, Rows-1); y++)
					{
						if (tile[y * Columns + x].hasContents() == Tile::Contents::Mine)
						{
							adj++;
						}
					}
				}
				tile[j * Columns + i].SetAdj(adj);
			}
		}
	}
}

MineField::Tile::Tile()
{
}

bool MineField::Tile::IsRevealed() const
{
	return isRevealed;
}

void MineField::Tile::OpenTile()
{
	isRevealed = true;
}

MineField::Tile::Contents MineField::Tile::hasContents()
{
	return contents;
}

void MineField::Tile::SetAdj(const int adjacent)
{
	adj = adjacent;
}

void MineField::Tile::PlaceBomb()
{
	assert(contents == Contents::Empty);

	contents = Contents::Mine;
}

int MineField::Tile::GetAdj() const
{
	return adj;
}

MineField::Tile::Suspicion MineField::Tile::IsSus()
{
	return isSus;
}

void MineField::Tile::CycleSus()
{
	switch (isSus)
	{
	case Tile::Suspicion::NoSus:
		isSus = Tile::Suspicion::Mine;
		break;

	case Tile::Suspicion::Mine:
		isSus = Tile::Suspicion::Unsure;
		break;
		
	case Tile::Suspicion::Unsure:
		isSus = Tile::Suspicion::NoSus;
		break;
	}
}

Vec<int> MineField::MouseToTile(const Vec<int> mvec) const
{
	return { (mvec.X - FieldPos.X) / TileSize.X , (mvec.Y - FieldPos.Y) / TileSize.Y };
}

Vec<int> MineField::GetClaims() const
{
	return {Claims, Mines};
}

Vec<int> MineField::GetScore() const
{
	return { Score, Mines };
}

int MineField::AdjMarked(const Vec<int> tpos) const
{
	//Counter for marked bombs adjacent
	int adjcount = 0;

	for (int i = max(tpos.X - 1, 0); i <= min((tpos.X + 1), Columns - 1); i++)
	{
		for (int j = max(tpos.Y - 1, 0); j <= min((tpos.Y + 1), Rows - 1); j++)
		{
			Tile& targ = tile[j * Columns + i];

			if (targ.IsSus() == MineField::Tile::Suspicion::Mine)
			{
				adjcount++;
			}
/*
			switch (targ.IsSus())
			{
			case MineField::Tile::Suspicion::NoSus:
				break;

			case MineField::Tile::Suspicion::Unsure:
				break;

			case MineField::Tile::Suspicion::Mine:
				adjcount++;
				break;
			}
*/
		}
	}

	return adjcount;
}

