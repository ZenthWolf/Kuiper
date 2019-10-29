#include "MineField.h"
#include <assert.h>

MineField::MineField()
{
}

MineField::MineField( int mines )
{
	Mines = mines;

	FieldPos = (VecI({ Graphics::ScreenWidth, Graphics::ScreenHeight }) - VecI({ Columns, Rows }) * TileSize.X) / 2;
}

void MineField::Draw(Graphics& gfx)
{
	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
		{
			VecI loc = { i * TileSize.X, j * TileSize.Y };

			if (!tile[j * Columns + i].IsRevealed())
			{

				Bev.ChangeBaseColor(CoveredColor);

				Bev.DrawBevBrick(RectI{ FieldPos + loc, FieldPos + loc + TileSize }.GetExpand(-1), 2, gfx);

				switch (tile[j * Columns + i].IsSus())
				{
				case Tile::Suspicion::Mine:
				{
					gfx.DrawCirc(FieldPos + loc + TileSize / 2 + VecI{ 1,1 }, TileSize.X / 2 - 1, BombColor);

					int hash = 0;
					while (hash <= TileSize.Y)
					{
						for (int x = (FieldPos + loc).X; x <= (FieldPos + loc + TileSize).X; x++)
						{
							gfx.PutPixel(x, (FieldPos + loc).Y + hash, CoveredColor);
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
						for (int x = (FieldPos + loc).X; x <= (FieldPos + loc + TileSize).X; x++)
						{
							gfx.PutPixel(x, (FieldPos + loc).Y + hash, Colors::Black);
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
					gfx.DrawRect(RectI{ FieldPos + loc, FieldPos + loc + TileSize }.GetExpand(-1), NumColor[tile[j * Columns + i].GetAdj()]);

					break;
				}

				case Tile::Contents::Mine:

					gfx.DrawRect(RectI{ FieldPos + loc, FieldPos + loc + TileSize }.GetExpand(-1), NumColor[0]);
					gfx.DrawCirc(FieldPos + loc + TileSize / 2 + VecI{1,1}, TileSize.X / 2 - 1, BombColor);
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
	if (tpos.X < Columns && tpos.Y < Rows && !tile[tpos.Y * Columns + tpos.X].IsRevealed() && tile[tpos.Y * Columns + tpos.X].IsSus() != Tile::Suspicion::Mine)
	{
		tile[tpos.Y * Columns + tpos.X].OpenTile();

		if (FreeMove)
		{
			assert(Mines < Rows * Columns);

			FreeMove = false;
			PlaceMines(Mines, rng);
		}

		if (tile[tpos.Y * Columns + tpos.X].hasContents() == Tile::Contents::Empty && tile[tpos.Y * Columns + tpos.X].GetAdj() == 0)
		{
			for (int i = min(abs(tpos.X - 1), tpos.X); i <= max(tpos.X, (tpos.X + 1) % Columns); i++)
			{
				for (int j = min(abs(tpos.Y - 1), tpos.Y); j <= max(tpos.Y, (tpos.Y + 1) % Rows); j++)
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
		int testtile = RandTile(rng);
		if (tile[testtile].hasContents() == Tile::Contents::Empty && !tile[testtile].IsRevealed())
		{
			tile[testtile].PlaceBomb();
			mines--;
		}
	}

	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
			if (tile[j * Columns + i].hasContents() == Tile::Contents::Empty)
			{
				int adj = 0;
				for (int x = min(abs(i - 1), i); x <= max(i, (i + 1) % Columns); x++)
				{
					for (int y = min(abs(j - 1), j); y <= max(j, (j + 1) % Rows); y++)
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

void MineField::Tile::CloseTile()
{
	isRevealed = false;
	contents = Contents::Empty;
}

MineField::Tile::Suspicion MineField::Tile::IsSus()
{
	return isSus;
}

void MineField::Tile::UnSus()
{
	isSus = Tile::Suspicion::NoSus;
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

void MineField::ResetField()
{
	for (int i = 0; i < Rows * Columns; i++)
	{
		tile[i].SetAdj(0);
		tile[i].CloseTile();
		tile[i].UnSus();
		FreeMove = true;
	}
}
