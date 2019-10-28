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

void Beveler::DrawOuterBev(RectF& Rect, int size, Graphics& gfx)
{
	/* LeftSide */
	gfx.DrawDLIsoTri(int(Rect.X0), int(Rect.Y0), size, LeftColor);
	gfx.DrawRect(RectF(Rect.X0, Rect.Y0 + float(size), Rect.X0 + float(size) - 1.0f, Rect.Y1 - float(size)), LeftColor);
	gfx.DrawULIsoTri(int(Rect.X0), int(Rect.Y1) - size + 1, size, LeftColor);

	/* TopSide */
	gfx.DrawURIsoTri(int(Rect.X0), int(Rect.Y0), size, TopColor);
	gfx.DrawRect(RectF(Rect.X0 + float(size), Rect.Y0, Rect.X1 - float(size), Rect.Y0 + float(size) - 1.0f), TopColor);
	gfx.DrawULIsoTri(int(Rect.X1) - size, int(Rect.Y0), size, TopColor);

	/* RightSide */
	gfx.DrawDRIsoTri(int(Rect.X1) - size, int(Rect.Y0), size, RightColor);
	gfx.DrawRect(RectF(Rect.X1 - float(size) + 1.0f, Rect.Y0 + float(size), Rect.X1, Rect.Y1 - float(size)), RightColor);
	gfx.DrawURIsoTri(int(Rect.X1) - size + 1, int(Rect.Y1) - size, size, RightColor);

	/* BotSide */
	gfx.DrawDRIsoTri(int(Rect.X0), int(Rect.Y1) - size + 1, size, BotColor);
	gfx.DrawRect(RectF(Rect.X0 + float(size), Rect.Y1 - float(size) + 1.0f, Rect.X1 - float(size), Rect.Y1 ), BotColor);
	gfx.DrawDLIsoTri(int(Rect.X1) - size + 1, int(Rect.Y1) - size + 1, size, BotColor);
}

void Beveler::DrawInnerBev(RectF& Rect, int size, Graphics& gfx)
{
	/* LeftSide */
	gfx.DrawDLIsoTri(int(Rect.X0), int(Rect.Y0), size, RightColor);
	gfx.DrawRect(RectF(Rect.X0, Rect.Y0 + float(size), Rect.X0 + float(size) - 1.0f, Rect.Y1 - float(size)), RightColor);
	gfx.DrawULIsoTri(int(Rect.X0), int(Rect.Y1) - size + 1, size, RightColor);

	/* TopSide */
	gfx.DrawURIsoTri(int(Rect.X0), int(Rect.Y0), size, BotColor);
	gfx.DrawRect(RectF(Rect.X0 + float(size), Rect.Y0, Rect.X1 - float(size), Rect.Y0 + float(size) - 1.0f), BotColor);
	gfx.DrawULIsoTri(int(Rect.X1) - size, int(Rect.Y0), size, BotColor);

	/* RightSide */
	gfx.DrawDRIsoTri(int(Rect.X1) - size, int(Rect.Y0), size, LeftColor);
	gfx.DrawRect(RectF(Rect.X1 - float(size) + 1.0f, Rect.Y0 + float(size), Rect.X1, Rect.Y1 - float(size)), LeftColor);
	gfx.DrawURIsoTri(int(Rect.X1) - size + 1, int(Rect.Y1) - size, size, LeftColor);

	/* BotSide */
	gfx.DrawDRIsoTri(int(Rect.X0), int(Rect.Y1) - size + 1, size, TopColor);
	gfx.DrawRect(RectF(Rect.X0 + float(size), Rect.Y1 - float(size) + 1.0f, Rect.X1 - float(size), Rect.Y1), TopColor);
	gfx.DrawDLIsoTri(int(Rect.X1) - size + 1, int(Rect.Y1) - size + 1, size, TopColor);
}

void Beveler::DrawBevBrick(RectF& Rect, int size, Graphics& gfx)
{
	DrawInnerBev(Rect, size, gfx);
	gfx.DrawRect(Rect.GetExpand(-float(size)), BaseColor);
}

void Beveler::DrawBevBorder(RectF& Rect, int size, Graphics& gfx)
{
	DrawOuterBev(Rect.GetExpand(float(2 * size)), size, gfx);
	DrawInnerBev(Rect.GetExpand(float(size)), size, gfx);
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

