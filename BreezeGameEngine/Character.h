#pragma once

#include "Animation.h"
#include "Vec.h"

class Character
{
private:
	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};
public:
	Character(const Vec& pos);
	void Draw( Graphics& gfx ) const;
	void SetDir( const Vec& dir );
	void Update(float dt);
private:
	Surface sprite;
	Vec pos;
	Vec vel = { 0.0f, 0.0f };
	std::vector<Animation> animation;
	Sequence curSeq = Sequence::StandingDown;
	float speed = 60.0f;
};