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
		float Deviance[4]; //0-4 Left, Up, Right, Down
		float MinDev;

		float UpDeviance = charcoll.Y1 - gndcoll.Y0;
		if (UpDeviance > 0.0f)
		{
			MinDev = UpDeviance;
		}
		
		float LeftDeviance = charcoll.X1 - gndcoll.X0;
		if (LeftDeviance > 0.0f && LeftDeviance < MinDev)
		{
			MinDev = LeftDeviance;
		}
		float DownDeviance = gndcoll.Y1 - charcoll.Y0;
		if (DownDeviance > 0.0f && DownDeviance < MinDev)
		{
			MinDev = DownDeviance;
		}
		float RightDeviance = gndcoll.X1 - charcoll.X0;
		if (RightDeviance > 0.0f && RightDeviance < MinDev)
		{
			MinDev = RightDeviance;
		}

		if (MinDev == UpDeviance)
		{
			pos.Y += UpDeviance;
		}
		else if (MinDev == LeftDeviance)
		{
			pos.X -= LeftDeviance;
		}
		else if (MinDev == DownDeviance)
		{
			pos.Y -= DownDeviance;
		}
		else if(MinDev == RightDeviance)
		{ 
			pos.X += RightDeviance;
		}
		else
		{
			//There probably should be an error or something here...
		}
	}

	animation[(int)curSeq].Update(dt);
}

Rect<float> Character::GetCollBox()
{
	return Rect<float>( pos + Vec<float>(25.0f, 20.0f), pos + Vec<float>(65.0f, 70.0f) );
}
