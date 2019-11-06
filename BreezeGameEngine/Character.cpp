#include "Character.h"

Character::Character(const Vec<float>& pos)
	:pos(pos), sprite("Images//link90x90.bmp")
{
	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animation.emplace_back(90, 90*i, 90, 90, 4, sprite, 0.16f);
	}

	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animation.emplace_back(90, 90 * (i-int(Sequence::StandingLeft)), 90, 90, 1, sprite, 0.16f);
	}
}

void Character::Draw(Graphics& gfx, bool showHitBox) const
{
	if (!showHitBox)
	{
		animation[(int)curSeq].Draw(Vec<int>(pos), gfx, showHitBox);
	}
	else
	{
		animation[(int)curSeq].Draw(Vec<int>(pos), gfx, showHitBox);
	}
}

void Character::Draw(Graphics& gfx, Color sub) const
{
	animation[(int)curSeq].Draw(Vec<int>(pos), gfx, sub);
}

void Character::SetDir(const Vec<float>& dir)
{
	if (dir.X > 0.0f)
	{
		curSeq = Sequence::WalkingRight;
	}
	else if (dir.X < 0.0f)
	{
		curSeq = Sequence::WalkingLeft;
	}
	else if (dir.Y > 0.0f)
	{
		curSeq = Sequence::WalkingDown;
	}
	else if (dir.Y < 0.0f)
	{
		curSeq = Sequence::WalkingUp;
	}
	else
	{
		if (vel.X > 0.0f)
		{
			curSeq = Sequence::StandingRight;
		}
		else if (vel.X < 0.0f)
		{
			curSeq = Sequence::StandingLeft;
		}
		else if (vel.Y > 0.0f)
		{
			curSeq = Sequence::StandingDown;
		}
		else if (vel.Y < 0.0f)
		{
			curSeq = Sequence::StandingUp;
		}
	}
	vel = dir * speed;
}

void Character::Update(float dt, Ground& gnd, Keyboard& kbd)
{
	Vec<float> dir = { 0.0f, 0.0f };
	if (kbd.KeyIsPressed(VK_UP))
	{
		dir.Y -= 1.0f;
	}
	if (kbd.KeyIsPressed(VK_DOWN))
	{
		dir.Y += 1.0f;
	}
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		dir.X -= 1.0f;
	}
	if (kbd.KeyIsPressed(VK_RIGHT))
	{
		dir.X += 1.0f;
	}

	SetDir(dir);

	pos += vel * dt;

	if ( hitBox().GetExpand( 0.0f, -18.0f).CollWith(gnd.Rect()) )
	{
	pos.Y = gnd.sayY() - 90.0f + 18.0f;
	}

	animation[(int)curSeq].Update(dt);
}

Rect<float> Character::hitBox()
{
	return { pos, pos + Vec<float>{90.0f, 90.0f} };
}
