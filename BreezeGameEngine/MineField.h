#pragma once

#include "Graphics.h"
#include "Beveler.h"
#include <random>

class MineField
{
public:
	MineField();
	MineField(int mines);
	void Draw(Graphics& gfx);
	void RevealTile(const VecI tpos, std::mt19937& rng);
	void PlaceMines(int mines, std::mt19937& rng);
	VecI MouseToTile(const VecI mvec) const;
	void ResetField();

private:
	class Tile 
	{
	public:
		Tile();
		enum class TileContents
		{
		    Empty,
			Mine
		};

	public:
		bool IsRevealed() const;
		void OpenTile();
		TileContents hasContents() const;
		void SetAdj(const int adjacent);
		void PlaceBomb();
		int GetAdj() const;
		void CloseTile();

	private:
		bool isRevealed = false;
		int adj = 5;
		TileContents contents = TileContents::Empty;
	};

	static constexpr int Columns = 30;
	static constexpr int Rows = 16;
    VecI TileSize = { 25, 25 };
	VecI FieldPos = { 0, 0 };

	Tile tile[Rows * Columns];

	Color CoveredColor = Colors::Gray;
	Color NumColor[9] = { Colors::White,
						 Colors::Red,
						 Color(240, 140, 40),
						 Colors::Yellow,
						 Colors::Green,
						 Colors::Cyan,
						 Colors::Blue,
						 Colors::Magenta,
						 Colors::Black
	                    };
	Color BombColor = Colors::Black;
	bool FreeMove = true;
	int Mines = 10;

	Beveler Bev;
};