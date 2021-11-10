#include "Coin.h"
#include "Mario.h"
#include "Utils.h"
#include "Game.h"
#include "PlayScence.h"
#include "Sprites.h"

CCoin::CCoin()
{
	this->SetState(COIN_STATE_EXIST);
	isAppear = true;
}

CCoin::CCoin(int tag)
{
	this->SetTag(tag);
	this->SetState(COIN_STATE_JUMPING);
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroyed)
		return;

		left = x;
		top = y;
		right = x + COIN_BBOX_WIDTH;
		bottom = y + COIN_BBOX_HEIGHT;

}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed)
		return;

	    CGameObject::Update(dt, coObjects);
		y += dy;

		if (this->tag == TAG_COIN_IN_BRICK) {
			switch (this->state)
			{
			case COIN_STATE_JUMPING:
				if (start_Y - y >= COIN_JUMP_MAX_DY) {
					this->SetState(COIN_STATE_FALLING);
				}
				break;
			case COIN_STATE_FALLING:
				if (start_Y - y <= COIN_JUMP_DISAPPEAR_DY) {
					isDestroyed = true;
				}
			default:
				break;
			}
		}
}

void CCoin::Render()
{
	if (isDestroyed)
		return;

		animation_set->at(COIN_ANI_GENERAL)->Render(x, y);
		RenderBoundingBox();	
}

void CCoin::SetState(int state)
{
	switch (state)
	{
	case COIN_STATE_JUMPING:
		vy = -COIN_JUMP_SPEED_Y;
		break;
	case COIN_STATE_FALLING:
		vy = COIN_JUMP_SPEED_Y;
		break;
	case COIN_STATE_NON_EXIST:
		vy = 0;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}
