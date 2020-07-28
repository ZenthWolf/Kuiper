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

void Character::Draw(Graphics& gfx) const
{
	animation[(int)curSeq].Draw( Vec<int>(pos), gfx );
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

void Character::Update(float dt, Ground gnd)
{
	pos += vel * dt;

	Rect<float> charcoll = GetCollBox();
	Rect<float> gndcoll = gnd.GetRect();

	if (charcoll.CollWith(gndcoll))
	{
		pos.Y = gndcoll.Y0 - 20.0f - (charcoll.height());
	}

	animation[(int)curSeq].Update(dt);
}

Rect<float> Character::GetCollBox()
{
	return Rect<float>( pos + Vec<float>(25.0f, 20.0f), pos + Vec<float>(65.0f, 70.0f) );
}
