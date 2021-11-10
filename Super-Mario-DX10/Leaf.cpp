#include "Leaf.h"
CLeaf::CLeaf()
{
	this->SetState(LEAF_STATE_FLY_UP);
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroyed)
		return;
	left = x;
	top = y;
	right = x + LEAF_BBOX_WIDTH;
	bottom = y + LEAF_BBOX_HEIGHT;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed)
		return;
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;

	switch (this->state)
	{
	case LEAF_STATE_FLY_UP:
		if (start_Y - y >= 50) {
			this->SetState(LEAF_STATE_FLY_DOWN_RIGHT);
		}
		break;
	case LEAF_STATE_FLY_DOWN_RIGHT:
		if (x - start_X >= 25) {
			this->SetState(LEAF_STATE_FLY_DOWN_LEFT);
		}
		break;
	case LEAF_STATE_FLY_DOWN_LEFT:
		if (start_X - x >= 25) {
			this->SetState(LEAF_STATE_FLY_DOWN_RIGHT);
		}
		break;
	default:
		break;
	}
}

void CLeaf::Render()
{
	if (isDestroyed)
		return;
	int ani = LEAF_ANI_FLY_LEFT;
	switch (this->state)
	{
	case LEAF_STATE_FLY_DOWN_LEFT:
		if (start_X - x >= 25) {
			this->SetState(LEAF_STATE_FLY_DOWN_RIGHT);
		}
		ani = LEAF_ANI_FLY_RIGHT;
		break;
	default:
		break;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	switch (state)
	{
	case LEAF_STATE_FLY_UP:
		vy = -LEAF_SPEED_Y;
		vx = 0;
		break;
	case LEAF_STATE_FLY_DOWN_LEFT:
		vy = LEAF_SPEED_Y;
		vx = -LEAF_SPEED_X;
		break;
	case LEAF_STATE_FLY_DOWN_RIGHT:
		vy = LEAF_SPEED_Y;
		vx = LEAF_SPEED_X;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}
