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

	RectF Left(gfx, 0.0f, 0.0f, 5.0f, float(gfx.ScreenHeight), Colors::White);
}