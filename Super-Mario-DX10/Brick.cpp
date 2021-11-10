#include "Brick.h"

void CBrick::Render()
{
	animation_set->at(BRICK_GENERAL_ANI)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

CBrick::CBrick(int tag, int isAbleToGoIn) {
	this->tag = tag;
	if (isAbleToGoIn == 0) {
		this->isAbleToGoIn = true;
	}
}



