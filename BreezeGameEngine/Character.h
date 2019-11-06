#pragma once

#include "Keyboard.h"
#include "Vec.h"
#include "Animation.h"
#include "Ground.h"

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
	Character(const Vec<float>& pos);
	void Draw(Graphics& gfx, bool showHitBox = false) const;
	void Draw(Graphics& gfx, Color sub) const;
	void SetDir( const Vec<float>& dir );
	void Update(float dt, Ground& gnd, Keyboard& kbd);
	Rect<float> hitBox();

private:
	Surface sprite;
	Vec<float> pos;
	Vec<float> vel = { 0.0f, 0.0f };
	std::vector<Animation> animation;
	Sequence curSeq = Sequence::StandingDown;
	float speed = 100.0f;
};