#include "Ground.h"

Ground::Ground(const Vec<float> v0, const Vec<float> v1)
	:box(v0,v1)
{

}

void Ground::Draw(Graphics& gfx) const
{
	gfx.DrawRect(box, col);
}