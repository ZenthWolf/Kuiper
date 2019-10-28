#include "MineField.h"
#include <assert.h>

MineField::MineField()
{
}

MineField::MineField(VecI fieldpos, int mines, std::mt19937& rng)
{
	assert(mines < Rows * Columns);

	FieldPos = fieldpos;

	PlaceMines(mines, rng);
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
			}
			else
			{
				switch (tile[j * Columns + i].hasContents())
				{
				case Tile::TileContents::Empty:
				{
					Bev.ChangeBaseColor(NumColor[tile[j * Columns + i].GetAdj()]);

					Bev.DrawBevBrick(RectI{ FieldPos + loc, FieldPos + loc + TileSize }.GetExpand(-1), 2, gfx);

					break;
				}

				case Tile::TileContents::Mine:

					gfx.DrawRect(RectI{ FieldPos + loc, FieldPos + loc + TileSize }.GetExpand(-1), NumColor[0]);
					gfx.DrawCirc(FieldPos + loc + TileSize / 2 + VecI{1,1}, TileSize.X / 2 - 1, BombColor);
					break;
				}
			}
		}
	}
}

void MineField::RevealTile(const VecI tpos)
{
	int X = (tpos.X - FieldPos.X) / TileSize.X;
	int Y = (tpos.Y - FieldPos.Y) / TileSize.Y;

	if (X < Columns && Y < Rows)
	{
		tile[Y * Columns + X].OpenTile();
	}
}

void MineField::PlaceMines(int mines, std::mt19937& rng)
{
	std::uniform_int_distribution<int> RandTile(0, Rows * Columns - 1);

	while (mines > 0)
	{
		int testtile = RandTile(rng);
		if (tile[testtile].hasContents() == Tile::TileContents::Empty)
		{
			tile[testtile].PlaceBomb();
			mines--;
		}
	}

	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
			if (tile[j * Columns + i].hasContents() == Tile::TileContents::Empty)
			{
				int adj = 0;
				for (int x = min(abs(i - 1), i); x <= max(i, (i + 1) % Columns); x++)
				{
					for (int y = min(abs(j - 1), j); y <= max(j, (j + 1) % Rows); y++)
					{
						if (tile[y * Columns + x].hasContents() == Tile::TileContents::Mine)
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

MineField::Tile::TileContents MineField::Tile::hasContents() const
{
	return contents;
}

void MineField::Tile::SetAdj(const int adjacent)
{
	adj = adjacent;
}

void MineField::Tile::PlaceBomb()
{
	assert(contents == TileContents::Empty);

	contents = TileContents::Mine;
}

int MineField::Tile::GetAdj() const
{
	return adj;
}
