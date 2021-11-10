#include "PiranhaPlant.h"
#include "Utils.h"
CPiranhaPlant::CPiranhaPlant()
{
	this->SetState(PIRANHA_STATE_UP);
}

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHA_BBOX_WIDTH;
	bottom = y + PIRANHA_BBOX_HEIGHT;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;

	switch (state)
	{
	case PIRANHA_STATE_UP:
		if (start_Y - y >= PIRANHA_HIGHEST_DY) {
			this->SetState(PIRANHA_STATE_DOWN);
		}
		break;
	case PIRANHA_STATE_DOWN:
		if (start_Y - y <= 0) {
			this->SetState(PIRANHA_STATE_UP);
		}
		break;
	default:
		break;
	}
}

void CPiranhaPlant::Render()
{
	int ani = PIRANHA_ANI_NORMAL;
	animation_set->at(ani)->Render(x, y+ EXTRA_PIRANHA_DY_MOVING_UP);
}

void CPiranhaPlant::SetState(int state)
{
	switch (state) {
	case PIRANHA_STATE_UP:
		vy = -PIRANHA_UP_SPEED;
		break;
	case PIRANHA_STATE_DOWN:
		vy = PIRANHA_UP_SPEED;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}
