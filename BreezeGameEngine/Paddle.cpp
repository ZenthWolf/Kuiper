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

void Paddle::Update(Keyboard& kbd)
{
	if (kbd.KeyIsPressed(VK_Up))
	{
		Vx = -1;
	}
	else if(kbd.KeyIsPressed(VK_Right))
}
