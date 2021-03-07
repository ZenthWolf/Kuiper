#pragma once

#include "Graphics.h"
#include "Beveler.h"
#include <random>

class MineField
{
public:
	enum class Difficulty
	{
		Beginner,
		Intermediate,
		Expert
	};

public:
	MineField( Difficulty level );
	~MineField();
	void SetField(int rows, int cols, int mines);
	void ClearField();
	void PrimeL();
	void PrimeR();
	bool CheckL() const;
	bool CheckR() const;
	bool CheckD(float dt);
	void ReliefL();
	void ReliefR();
	void Draw(Graphics& gfx);
	void RevealTile(const Vec<int> tpos, std::mt19937& rng);
	void RevealAdjacent(const Vec<int> tpos, std::mt19937& rng);
	void SusTile(const Vec<int> tpos);
	bool MineField::ValidMine(int targ);
	void PlaceMines(int mines, std::mt19937& rng);
	Vec<int> MouseToTile(const Vec<int> mvec) const;

private:
	class Tile 
	{
	public:
		Tile();
		enum class Contents
		{
			Empty,
			Mine
		};
		enum class Suspicion
		{
			NoSus,
			Mine,
			Unsure
		};

	public:
		bool IsRevealed() const;
		void OpenTile();
		Contents hasContents();
		void SetAdj(const int adjacent);
		void PlaceBomb();
		int GetAdj() const;
		void CloseTile();
		Suspicion IsSus();
		void UnSus();
		void CycleSus();

	private:
		bool isRevealed = false;
		Suspicion isSus = Suspicion::NoSus;
		int adj = -1;
		Contents contents = Contents::Empty;
	};
	int AdjMarked(const Vec<int> tpos) const;

private:

	int Columns;
	int Rows;
	bool LPrimed;
	bool RPrimed;
	bool DClickPrimed;
	float DClickTimer;
    Vec<int> TileSize;
	Vec<int> FieldPos = { -1,-1 };

	Tile* tile = nullptr;

	const Color CoveredColor = Colors::Gray;
	const Color NumColor[9] = { Colors::White,
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
	bool FreeMove;
	int Mines;

	Beveler Bev;
};