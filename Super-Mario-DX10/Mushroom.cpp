#include "Mushroom.h"
#include "Brick.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "Utils.h"
#include "PlayScence.h"
#include "Game.h"

CMushroom::CMushroom()
{
	this->SetState(MUSHROOM_STATE_IDLE);
}

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroyed)
		return;
	left = x;
	top = y;
	right = x + MUSHROOM_BBOX_WIDTH;
	bottom = y + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed)
		return;
	CGameObject::Update(dt);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		vy += ay * dt;
		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);
		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x += min_tx * dx + nx * PUSHBACK;
			y += min_ty * dy + ny * PUSHBACK;

			if (ny != 0) vy = 0;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				float mLeft, mTop, mRight, mBottom;
				GetBoundingBox(mLeft, mTop, mRight, mBottom);
				if (dynamic_cast<CBrick*>(e->obj)
					||dynamic_cast<CQuestionBrick*>(e->obj))
				{
					if (e->ny != 0)
					{
						vy = 0;
						ay = MUSHROOM_GRAVITY;
					}
				}
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		switch (this->state) {
		case MUSHROOM_STATE_IDLE:
			if (start_Y - y >= MUSHROOM_BBOX_HEIGHT) {
				this->SetState(MUSHROOM_STATE_MOVING);
			}
			break;
		default:
			break;
		}
}

void CMushroom::Render()
{
	if (isDestroyed)
		return;
	int ani = MUSHROOM_ANI_NORMAL;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CMushroom::SetState(int state)
{
	CMario* mario = {};
	mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vy = -MUSHROOM_Y_VELOCITY;
		vx = 0;
		break;
	case MUSHROOM_STATE_MOVING:
	{
		vx = mario->nx*MUSHROOM_X_VELOCITY;
		ay = MUSHROOM_GRAVITY;
		vy = 0;
		break;
	}
	case MUSHROOM_STATE_NON_EXIST:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);

}
