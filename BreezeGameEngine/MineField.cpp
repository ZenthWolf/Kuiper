#include "MineField.h"
#include <assert.h>

MineField::MineField( Difficulty level )
	:LPrimed(false), RPrimed(false)
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

	FieldPos = (VecI({ Graphics::ScreenWidth, Graphics::ScreenHeight }) - VecI({ Columns, Rows }) * TileSize.X) / 2;
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
			VecI loc = VecI{ i * TileSize.X, j * TileSize.Y } + FieldPos;

			if (!tile[j * Columns + i].IsRevealed())
			{

				Bev.ChangeBaseColor(CoveredColor);

				Bev.DrawBevBrick(RectI{ loc, loc + TileSize }.GetExpand(-1), 2, gfx);

				switch (tile[j * Columns + i].IsSus())
				{
				case Tile::Suspicion::Mine:
				{
					gfx.DrawCirc(loc + TileSize / 2 + VecI{ 1,1 }, TileSize.X / 2 - 1, BombColor);

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
					gfx.DrawRect(RectI{ loc, loc + TileSize }.GetExpand(-1), NumColor[tile[j * Columns + i].GetAdj()]);

					break;
				}

				case Tile::Contents::Mine:

					gfx.DrawRect(RectI{ loc, loc + TileSize }.GetExpand(-1), NumColor[0]);
					gfx.DrawCirc(loc + TileSize / 2 + VecI{1,1}, TileSize.X / 2 - 1, BombColor);
					break;
				}
			}
		}
	}

	Bev.DrawBevBorder(RectI({ (Graphics::ScreenWidth - Columns * TileSize.X) / 2, (Graphics::ScreenHeight - Rows * TileSize.Y) / 2 },
		                    { (Graphics::ScreenWidth + Columns * TileSize.X) / 2, (Graphics::ScreenHeight + Rows * TileSize.Y) / 2 }), 3, gfx);
}

void MineField::RevealTile(const VecI tpos, std::mt19937& rng)
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
	}
}

void MineField::SusTile(const VecI tpos)
{
	if (tpos.X < Columns && tpos.Y < Rows && !tile[tpos.Y * Columns + tpos.X].IsRevealed())
	{
		tile[tpos.Y * Columns + tpos.X].CycleSus();
	}
}

void MineField::PlaceMines(int mines, std::mt19937& rng)
{
	std::uniform_int_distribution<int> RandTile(0, Rows * Columns - 1);

	while (mines > 0)
	{
		int targ = RandTile(rng);
		if (tile[targ].hasContents() == Tile::Contents::Empty && !tile[targ].IsRevealed())
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

VecI MineField::MouseToTile(const VecI mvec) const
{
	return { (mvec.X - FieldPos.X) / TileSize.X , (mvec.Y - FieldPos.Y) / TileSize.Y };
}
