#include "Beveler.h"

Beveler::Beveler(Color base)
{

	float HighVal = float(    max(max(base.GetR(), base.GetB()), base.GetG())) * LeftFactor;

	if (int(HighVal) > 255)
	{
		base.SetR(int(float(base.GetR()) * 255.0f / HighVal));
		base.SetG(int(float(base.GetG()) * 255.0f / HighVal));
		base.SetB(int(float(base.GetB()) * 255.0f / HighVal));
	}


	BaseColor = base;
	
	LeftColor = Color(
		int(float(base.GetR()) * LeftFactor),
		int(float(base.GetG()) * LeftFactor),
		int(float(base.GetB()) * LeftFactor)
	);

	RightColor = Color(
		int(float(base.GetR()) * RightFactor),
		int(float(base.GetG()) * RightFactor),
		int(float(base.GetB()) * RightFactor)
	);

	TopColor = Color(
		int(float(base.GetR()) * TopFactor),
		int(float(base.GetG()) * TopFactor),
		int(float(base.GetB()) * TopFactor)
	);

	BotColor = Color(
		int(float(base.GetR()) * BotFactor),
		int(float(base.GetG()) * BotFactor),
		int(float(base.GetB()) * BotFactor)
	);
}

void Beveler::DrawOuterBev(Rect<int>& rect, int size, Graphics& gfx)
{
	/* LeftSide */
	gfx.DrawDLIsoTri((rect.X0), (rect.Y0), size, LeftColor);
	gfx.DrawRect(Rect<int>(rect.X0, rect.Y0 + (size), rect.X0 + (size) - 1, rect.Y1 - (size)), LeftColor);
	gfx.DrawULIsoTri((rect.X0), (rect.Y1) - size + 1, size, LeftColor);

	/* TopSide */
	gfx.DrawURIsoTri((rect.X0), (rect.Y0), size, TopColor);
	gfx.DrawRect(Rect<int>(rect.X0 + (size), rect.Y0, rect.X1 - (size), rect.Y0 + (size) - 1), TopColor);
	gfx.DrawULIsoTri((rect.X1) - size, (rect.Y0), size, TopColor);

	/* RightSide */
	gfx.DrawDRIsoTri((rect.X1) - size, (rect.Y0), size, RightColor);
	gfx.DrawRect(Rect<int>(rect.X1 - (size) + 1, rect.Y0 + (size), rect.X1, rect.Y1 - (size)), RightColor);
	gfx.DrawURIsoTri((rect.X1) - size + 1, (rect.Y1) - size, size, RightColor);

	/* BotSide */
	gfx.DrawDRIsoTri((rect.X0), (rect.Y1) - size + 1, size, BotColor);
	gfx.DrawRect(Rect<int>(rect.X0 + (size), rect.Y1 - (size) + 1, rect.X1 - (size), rect.Y1 ), BotColor);
	gfx.DrawDLIsoTri((rect.X1) - size + 1, (rect.Y1) - size + 1, size, BotColor);
}

void Beveler::DrawInnerBev(Rect<int>& rect, int size, Graphics& gfx)
{
	/* LeftSide */
	gfx.DrawDLIsoTri((rect.X0), (rect.Y0), size, RightColor);
	gfx.DrawRect(Rect<int>(rect.X0, rect.Y0 + (size), rect.X0 + (size) - 1, rect.Y1 - (size)), RightColor);
	gfx.DrawULIsoTri((rect.X0), (rect.Y1) - size + 1, size, RightColor);

	/* TopSide */
	gfx.DrawURIsoTri((rect.X0), (rect.Y0), size, BotColor);
	gfx.DrawRect(Rect<int>(rect.X0 + (size), rect.Y0, rect.X1 - (size), rect.Y0 + (size) - 1), BotColor);
	gfx.DrawULIsoTri((rect.X1) - size, (rect.Y0), size, BotColor);

	/* RightSide */
	gfx.DrawDRIsoTri((rect.X1) - size, (rect.Y0), size, LeftColor);
	gfx.DrawRect(Rect<int>(rect.X1 - (size) + 1, rect.Y0 + (size), rect.X1, rect.Y1 - (size)), LeftColor);
	gfx.DrawURIsoTri((rect.X1) - size + 1, (rect.Y1) - size, size, LeftColor);

	/* BotSide */
	gfx.DrawDRIsoTri((rect.X0), (rect.Y1) - size + 1, size, TopColor);
	gfx.DrawRect(Rect<int>(rect.X0 + (size), rect.Y1 - (size) + 1, rect.X1 - (size), rect.Y1), TopColor);
	gfx.DrawDLIsoTri((rect.X1) - size + 1, (rect.Y1) - size + 1, size, TopColor);
}

void Beveler::DrawBevBrick(Rect<int>& rect, int size, Graphics& gfx)
{
	DrawInnerBev(rect, size, gfx);
	gfx.DrawRect(rect.GetExpand(-(size)), BaseColor);
}

void Beveler::DrawBevBorder(Rect<int>& rect, int size, Graphics& gfx)
{
	DrawOuterBev(rect.GetExpand((2 * size)), size, gfx);
	DrawInnerBev(rect.GetExpand((size)), size, gfx);
}

void Beveler::ChangeBaseColor(Color newbase)
{
	float HighVal = float(max(max(newbase.GetR(), newbase.GetB()), newbase.GetG())) * LeftFactor;

	if (int(HighVal) > 255)
	{
		newbase.SetR(int(float(newbase.GetR()) * 255.0f / HighVal));
		newbase.SetG(int(float(newbase.GetG()) * 255.0f / HighVal));
		newbase.SetB(int(float(newbase.GetB()) * 255.0f / HighVal));
	}

	BaseColor = newbase;

	LeftColor = Color(
		int(float(newbase.GetR()) * LeftFactor),
		int(float(newbase.GetG()) * LeftFactor),
		int(float(newbase.GetB()) * LeftFactor)
	);

	RightColor = Color(
		int(float(newbase.GetR()) * RightFactor),
		int(float(newbase.GetG()) * RightFactor),
		int(float(newbase.GetB()) * RightFactor)
	);

	TopColor = Color(
		int(float(newbase.GetR()) * TopFactor),
		int(float(newbase.GetG()) * TopFactor),
		int(float(newbase.GetB()) * TopFactor)
	);

	BotColor = Color(
		int(float(newbase.GetR()) * BotFactor),
		int(float(newbase.GetG()) * BotFactor),
		int(float(newbase.GetB()) * BotFactor)
	);
}

