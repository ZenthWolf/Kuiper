#include "Paddle.h"
#include "Keyboard.h"

Paddle::Paddle(Graphics& gfx, Keyboard& kbd):gfx(gfx), kbd(kbd), Rect(gfx)
{
}

Paddle::Paddle(Graphics& gfx, Keyboard& kbd, float x, float y, float w, float h, Color c)
	:gfx(gfx), kbd(kbd), Rect(gfx, x, y, w, h, c)
{
}

void Paddle::Draw()
{
	Rect.Draw();
}

void Paddle::Update(Keyboard& kbd, float dt)
{
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		Rect.Pos.X -= Vx * dt * 60.0f;
	}
	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		Rect.Pos.X += Vx * dt * 60.0f;
	}

/*** Test Rects for Wall Behavior ***/
	RectF Left(gfx, 0.0f, 0.0f, 5.0f, float(gfx.ScreenHeight), Colors::White);
	RectF Right(gfx, float(gfx.ScreenWidth) - 5.0f, 0.0f, 5.0f, float(gfx.ScreenHeight), Colors::White);
//	RectF Top(gfx, 0.0f, 0.0f, float(gfx.ScreenWidth), 5.0f, Colors::White);
//	RectF Bot(gfx, 0.0f, float(gfx.ScreenHeight) - 5.0f, float(gfx.ScreenWidth), 5.0f, Colors::White);

	if (Rect.CollWith(Left))
	{
		Rect.Pos.X = 5.0f;
	}
	else if (Rect.CollWith(Right))
	{
		Rect.Pos.X = float(gfx.ScreenWidth) - 5.0f - Rect.W;
	}
}