#include "Character.h"

Character::Character(const Vec& pos)
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

void Character::Draw(Graphics& gfx) const
{
	animation[(int)curSeq].Draw(VecI(pos), gfx);
}

void Character::SetDir(const Vec& dir)
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

void Character::Update(float dt)
{
	pos += vel * dt;
	animation[(int)curSeq].Update(dt);
}
