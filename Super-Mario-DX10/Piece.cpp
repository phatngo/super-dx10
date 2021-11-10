#include "Piece.h"
#include "Coin.h"
#include "Mario.h"
#include "Utils.h"
#include "Game.h"
#include "PlayScence.h"

CPiece::CPiece(float ax, float ay)
{
	this->ax = ax;
	this->ay = ay;
	SetState(PIECE_STATE_FLYING);
}


void CPiece::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDestroyed) {
		left = x;
		top = y;
		right = x + PIECE_BBOX_WIDTH;
		bottom = y + PIECE_BBOX_HEIGHT;
	}
}

void CPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed)
		return;
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;

	switch (this->state) {
	    case PIECE_STATE_FLYING:
		    if (appearanceTimer.ElapsedTime() >= PIECE_TIME && appearanceTimer.IsStarted()) {
			      this->SetState(PIECE_STATE_NON_EXIST);
		}
	}
}

void CPiece::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(PIECE_ANI_FLYING)->Render(x,y);
	RenderBoundingBox();
}

void CPiece::SetState(int state)
{
	switch (state)
	{
	case PIECE_STATE_FLYING:
		vx = this->ax * PIECE_SPEED;
		vy = this->ay * PIECE_SPEED;
		appearanceTimer.Start();
		break;
	case PIECE_STATE_NON_EXIST:
		vy = 0;
		vx = 0;
		isDestroyed = true;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

