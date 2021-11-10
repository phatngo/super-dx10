
#include "Koopas.h"
#include "FirePiranhaPlant.h"
#include "Brick.h"
#include "Block.h"
#include "QuestionBrick.h"
#include "FlashAnimationBrick.h"
#include "Goomba.h"
#include "Utils.h"
#include "Timer.h"
#include "PlayScence.h"
#include "Game.h"
#include "Mario.h"
#include "EffectPoint.h"

CKoopas::CKoopas(int tag)
{
	nx = -1;
	SetState(KOOPAS_STATE_WALKING);
	this->SetTag(tag);
	this->SetType(MOVING);

}
void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SPINNING || state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_SHAKE)
	{
		bottom = y + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;

}
void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CMario* mario = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
	bool isMarioThrow = mario->IsThrow();

	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (state != KOOPAS_STATE_DEATH && !isHold)
		vy += ay * dt;

	coEvents.clear();

	if (state != KOOPAS_STATE_DEATH)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty;
		float nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		float x0 = x, y0 = y;
		x = x0 + min_tx * dx + nx * PUSHBACK;
		y = y0 + min_ty * dy + ny * PUSHBACK;

		float mLeft, mTop, mRight, mBottom;
		// Collision logic with others
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			float oLeft, oTop, oRight, oBottom;
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj != NULL)

				GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny > 0)
					y = y0;
				if (koopas->state == KOOPAS_STATE_SPINNING)
				{
					if (tag == KOOPAS_GREEN_PARA)
						tag = KOOPAS_GREEN;
					this->SetState(KOOPAS_STATE_DEATH);
				}
				else
				{
					x = x0;
					y = y0;
					if ((koopas->state == KOOPAS_STATE_SHELL_UP || koopas->state == KOOPAS_STATE_IN_SHELL)
						&& state == KOOPAS_STATE_WALKING)
					{
						x = x0 + dx;
						y = y0 + dy;
					}
					if (state == KOOPAS_STATE_WALKING && koopas->state == KOOPAS_STATE_WALKING)
					{
						x = x0 + dx;
						if (e->ny < 0)
							y = y0 + dy;
						this->vx = -this->vx;
						this->nx = -this->nx;
						koopas->vx = -koopas->vx;
						koopas->nx = -koopas->nx;
					}
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetState() != GOOMBA_STATE_DIE && this->GetState() == KOOPAS_STATE_SPINNING)
				{
					if (goomba->GetState() != GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS) {
						goomba->SetState(GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS);
						goomba->SetKillingKoopasDiretion(this->nx);
					}
					vy = 0;
					ay = KOOPAS_GRAVITY;
					if (!goomba->IsTouchedByKoopas()) {
						mario->AddPoint(x, y, EFFECT_POINT_400);
					}
					goomba->SetIsTouchedByKoopas();
				}
				if (e->ny > 0) {
					ay = 0;
					vx += ax * dt;
					x = x0 + dx;
				}
				vx += ax * dt;
				x = x0 + dx;
			}
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* object = dynamic_cast<CBrick*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (e->ny != 0) {
					vy = 0;
					ay = KOOPAS_GRAVITY;
					if (state == KOOPAS_STATE_SHELL_UP)
						vx = 0;
					if (tag == KOOPAS_GREEN_PARA)
					{
						y = e->obj->y - KOOPAS_BBOX_HEIGHT;
						vy = -KOOPAS_JUMP_SPEED;
						if (marioX <= x) {
							this->nx = -1;
						}
						else {
							this->nx = 1;
						}
						vx = this->nx * KOOPAS_WALKING_SPEED;
					}
				}
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
					{
						vx = -vx;
						this->nx = -this->nx;
					}
				}
			}
			if (dynamic_cast<CQuestionBrick*>(e->obj) && state == KOOPAS_STATE_SPINNING && e->nx != 0
				//&& ceil(mBottom) != oTop
				)
			{
				if (e->obj->state != QUESTION_BRICK_STATE_STOP)
				{
					if (e->obj->state == QUESTION_BRICK_STATE_IDLE)
						e->obj->SetState(QUESTION_BRICK_STATE_JUMPING);
				}
				vx = -vx;
				this->nx = -this->nx;
			}

			if (dynamic_cast<CBlock*>(e->obj))
			{
				if (e->ny < 0)
				{
					vy = 0;
					ay = KOOPAS_GRAVITY;
					if (state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_SHAKE)
						vx = 0;
					if (tag == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
					{
						if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
						if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
					}
					if (tag == KOOPAS_GREEN_PARA)
					{
						y = e->obj->y - KOOPAS_BBOX_HEIGHT;
						vy = -KOOPAS_JUMP_SPEED;
						if (marioX <= x) {
							this->nx = -1;
						}
						else {
							this->nx = 1;
						}
						vx = this->nx * KOOPAS_WALKING_SPEED;

					}
				}
				else
				{
					if (e->nx != 0)
						x = x0 + dx;
					if (state == KOOPAS_STATE_SHELL_UP && e->ny > 0)
						y = y0 + dy;
				}
			}
			else if (dynamic_cast<CFlashAnimationBrick*>(e->obj))
			{
				CFlashAnimationBrick* flashAnimationBrick = dynamic_cast<CFlashAnimationBrick*>(e->obj);
				if (e->ny < 0)
				{
					vy = 0;
					ay = KOOPAS_GRAVITY;
					if (state == KOOPAS_STATE_SHELL_UP)
						vx = 0;
					if (tag == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
					{
						if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
						if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
					}
					if (tag == KOOPAS_GREEN_PARA)
					{
						y = e->obj->y - KOOPAS_BBOX_HEIGHT;
						vy = -KOOPAS_JUMP_SPEED;
					}

				}
				else
				{
					if (e->nx != 0) {
						vx = -vx;
						this->nx = -this->nx;
						flashAnimationBrick->SetState(FLASH_BRICK_STATE_NON_EXIST);
						flashAnimationBrick->SetIsDestroyed();
					}
					if (state == KOOPAS_STATE_SHELL_UP && e->ny > 0)
						y = y0 + dy;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	if (this->x <= 0)
	{
		if (state == KOOPAS_STATE_SPINNING)
		{
			this->nx = -this->nx;
			vx = this->nx * KOOPAS_WALKING_SPEED * KOOPAS_EXTRA_SPEED;
		}
		else {
			this->nx = -this->nx;
			vx = this->nx * KOOPAS_WALKING_SPEED;
		}
	}

	if (state == KOOPAS_STATE_IN_SHELL) {
		if (!isHold) {
			if (shellTimer.ElapsedTime() >= KOOPAS_SHELL_TIME && shellTimer.IsStarted()) {
				SetState(KOOPAS_STATE_SHAKE);
				shellTimer.Reset();
			}
		}
	}
    else if (state == KOOPAS_STATE_SHAKE) {
		if (respawnTimer.ElapsedTime() >= KOOPPAS_RESPAWN_TIME && respawnTimer.IsStarted()) {
			SetState(KOOPAS_STATE_WALKING);
			respawnTimer.Reset();
		}
	}

	if (state != KOOPAS_STATE_WALKING)
		koopas_y = y;
	else
		koopas_y = y + KOOPAS_DY_TRANSFROM_FROM_WALKING_TO_IN_SHELL;

	if (isHold && !isMarioThrow) {
		switch (mario->GetLevel())
		{
		case MARIO_LEVEL_SMALL:
			if (mario->nx > 0)
				this->x = marioX + KOOPAS_BBOX_WIDTH;
			else
				this->x = marioX - KOOPAS_BBOX_WIDTH;
			this->y = marioY - (MARIO_SMALL_BBOX_HEIGHT / 2);
			break;
		case MARIO_LEVEL_BIG:
			if (mario->nx > 0)
				this->x = marioX + KOOPAS_BBOX_WIDTH + 1;
			else
				this->x = marioX - KOOPAS_BBOX_WIDTH;
			this->y = marioY + (MARIO_BIG_BBOX_HEIGHT / 3);
			break;
		case MARIO_LEVEL_TAIL:
			if (mario->nx > 0)
				this->x = marioX + KOOPAS_BBOX_WIDTH + 1;
			else
				this->x = marioX - KOOPAS_BBOX_WIDTH;
			this->y = marioY + (MARIO_TAIL_BBOX_HEIGHT / 3);
			break;
		default:
			break;
		}
	}
	else if (isHold && isMarioThrow) {
		this->isHold = false;
		this->SetState(KOOPAS_STATE_SPINNING);
	}

}
void CKoopas::Render()
{
	int ani = -1;
	if (state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_DEATH) {
		ani = KOOPAS_ANI_SHELL_UP;
		this->SetState(KOOPAS_STATE_IN_SHELL);
	}
	else if (state == KOOPAS_STATE_IN_SHELL) {
		ani = KOOPAS_ANI_SHELL;
	}
	else if (state == KOOPAS_STATE_SPINNING)
	{
		if (vx < 0)
			ani = KOOPAS_ANI_SPIN_LEFT;
		else
			ani = KOOPAS_ANI_SPIN_RIGHT;
	}
	else
	{
		if (nx < 0)
			ani = KOOPAS_ANI_WALKING_LEFT;
		else
			ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	if (tag == KOOPAS_GREEN_PARA || tag == KOOPAS_RED_PARA)
		if (nx < 0)
			ani = KOOPAS_ANI_PARA_LEFT;
		else
			ani = KOOPAS_ANI_PARA_RIGHT;
	if (state == KOOPAS_STATE_SHAKE) {
		ani = KOOPAS_ANI_SHAKE;
	}
	animation_set->at(ani)->Render(x, koopas_y);
	RenderBoundingBox();
}
void CKoopas::SetState(int state)
{

	CGameObject* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();

	switch (state)
	{
	
	case KOOPAS_STATE_WALKING:
		y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT;
		if (tag == KOOPAS_RED_PARA)
		{
			vx = 0;
			vy = KOOPAS_RED_SPEED;
		}
		if (tag == KOOPAS_GREEN || tag == KOOPAS_RED)
			vx = nx * KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_SPINNING:
		vx = player->nx* KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_IN_SHELL:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT;
		vx = 0;
		if (!isHold) {
			shellTimer.Start();
		}
		vy = 0;
		break;
	case KOOPAS_STATE_DEATH:
		vy = 0;
		vx = 0;
		SetType(IGNORE_DEFINE);
		break;
	case KOOPAS_STATE_SHAKE:
		vx = 0;
		respawnTimer.Start();
		vy = 0;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}
bool CKoopas::CalRevivable()
{
	/*Camera* cam = Camera::GetInstance();
	if (!cam->isAreaCamera(x, y))
		return false;*/
	respawnTimer.Start();
	return true;
}
bool CKoopas::CalTurnable(LPGAMEOBJECT object, vector<LPGAMEOBJECT>* coObjects)
{
	/*Camera* cam = Camera::GetInstance();
	if (!cam->isAreaCamera(x, y))
	{
		temp = 1;
		return false;
	}*/
	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<CFlashAnimationBrick*>(coObjects->at(i)) || dynamic_cast<CBlock*>(coObjects->at(i)))
			if (abs(coObjects->at(i)->y == object->y))
			{
				if (nx > 0)
					if (coObjects->at(i)->x > object->x && coObjects->at(i)->x - BRICK_BBOX_WIDTH < object->x + BRICK_BBOX_WIDTH)
					{
						temp = 2;
						return false;
					}
				if (nx < 0)
					if (coObjects->at(i)->x + BRICK_BBOX_WIDTH > object->x - BRICK_BBOX_WIDTH && coObjects->at(i)->x < object->x)
					{
						temp = 3;
						return false;
					}
			}
	temp = 4;
	return true;
}
void CKoopas::Reset()
{
	x = start_x;
	y = start_y;
	nx = -1;
	SetState(KOOPAS_STATE_WALKING);
	tag = start_tag;
	type = MOVING;
	//isEnable = true;
}
