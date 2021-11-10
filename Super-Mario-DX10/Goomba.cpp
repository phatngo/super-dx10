#include "Goomba.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Game.h"
#include "Utils.h"
#include "PlayScence.h"
#include "EffectPoint.h"
#include "Block.h"
#include "Mario.h"

CGoomba::CGoomba(int tag)
{
	if (tag == GOOMBA_TAG_YELLOW) {
		SetState(GOOMBA_STATE_YELLOW_WALKING);
		this->SetTag(GOOMBA_TAG_YELLOW);
	}
	else {
		SetState(GOOMBA_STATE_FALLING_LOW);
		this->SetTag(GOOMBA_TAG_RED);
		lowFallingTime = 0;
		isOnGround = false;
		isQuestionBrickAboveTouched = false;
		isTouchedAnotherGoomba = false;
		isQuestionBrickBelowTouched = false;
	}
}
void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroyed)
		return;
	left = x;
	top = y;

	if (tag == GOOMBA_TAG_YELLOW) {
		right = x + GOOMBA_BBOX_NORMAL_WIDTH;

		if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_NON_EXIST)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_NORMAL_HEIGHT;
	}
	else {
		switch (state) {
		case GOOMBA_STATE_RED_WALK_WING:
			right = x + GOOMBA_BBOX_RED_WALK_WING_WIDTH;
			bottom = y + GOOMBA_BBOX_RED_WALK_WING_HEIGHT;
			break;
		case GOOMBA_STATE_RED_WALKING:
			right = x + GOOMBA_BBOX_NORMAL_WIDTH;
			bottom = y + GOOMBA_BBOX_NORMAL_HEIGHT;
			break;
		case GOOMBA_STATE_RED_DIE:
			right = x + GOOMBA_BBOX_NORMAL_WIDTH;
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
			break;
		default:
			right = x + GOOMBA_BBOX_RED_UNWALK_WIDTH;
			bottom = y + GOOMBA_BBOX_RED_UNWALK_HEIGHT;
			break;
		}
	}
}
void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed)
		return;
	CGameObject::Update(dt);

	if (tag == GOOMBA_TAG_YELLOW)
		updateYellowGoomba(dt, coObjects);
	else
		updateRedGoomba(dt, coObjects);

	CMario* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
	if (tag == GOOMBA_TAG_YELLOW) {
		if (state == GOOMBA_STATE_DIE) {
			//After being killed, goomba becomes dead then delay for a while, then disappears
			if (this->transformToNonExistTimer.ElapsedTime() >= GOOMBA_DELAY_TIME && this->transformToNonExistTimer.IsStarted()) {
				//this->SetState(GOOMBA_STATE_NON_EXIST);
				isDestroyed = true;
				player->AddPoint(this->x, this->y - GOOMBA_BBOX_NORMAL_HEIGHT);
				transformToNonExistTimer.Reset();
			}
		}
		if (state == GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS) {
			if (start_Y - y >= 25)
				this->SetState(GOOMBA_STATE_FALLING_KILLED_BY_KOOPAS);
		}
	}
	else {
		if (isOnGround)
		{
			//when red goomba is on the ground
			switch (state)
			{
			case GOOMBA_STATE_FALLING_LOW:
				if (lowFallingTime > MAX_LOW_FALLING_TIME) {
					lowFallingTime = 0;
					this->SetState(GOOMBA_STATE_JUMPING_HIGH);
				}
				else {
					this->SetState(GOOMBA_STATE_JUMPING_LOW);
				}
				isOnGround = false;
				break;
			case GOOMBA_STATE_FALLING_HIGH:
				this->SetState(GOOMBA_STATE_RED_WALK_WING);
				break;
			case GOOMBA_STATE_FALLING_AWAY_FROM_QUESTION_BRICK:
				this->SetState(GOOMBA_STATE_RED_WALK_WING);
				break;
			case GOOMBA_STATE_RED_WALK_WING:
				//After falling down from a high jump, red goomba will walk with a wing for a while then have a low jump
				//Timer is for measuring the time for walking with a wing
				if (redWalkWingTimer.ElapsedTime() >= GOOMBA_WALK_WING_TIME && redWalkWingTimer.IsStarted()) {
					this->SetState(GOOMBA_STATE_JUMPING_LOW);
					isOnGround = false;
				}
				break;
			case GOOMBA_STATE_RED_DIE:
				//After being killed, goomba becomes dead then delay for a while, then disappears
				if (transformToNonExistTimer.ElapsedTime() >= GOOMBA_DELAY_TIME && transformToNonExistTimer.IsStarted()) {
					//this->SetState(GOOMBA_STATE_NON_EXIST);
					isDestroyed = true;
					player->AddPoint(this->x, this->y - GOOMBA_BBOX_NORMAL_HEIGHT, EFFECT_POINT_800);
				}
				break;
			default:
				break;
			}
		}
		else if (isQuestionBrickAboveTouched) {
			//when red goomba touches the question brick above its head
			switch (this->state)
			{
			case GOOMBA_STATE_JUMPING_HIGH:
				this->SetState(GOOMBA_STATE_FALLING_HIGH);
				break;
			case GOOMBA_STATE_JUMPING_LOW:
				this->SetState(GOOMBA_STATE_FALLING_LOW);
				break;
			default:
				break;
			}
			isQuestionBrickAboveTouched = false;
		}
		else if (isQuestionBrickBelowTouched) {
			SetState(GOOMBA_STATE_JUMPING_AWAY_FROM_QUESTION_BRICK);
			isQuestionBrickBelowTouched = false;
		}
		else if (isTouchedAnotherGoomba) {
			switch (this->state)
			{
			case GOOMBA_STATE_FALLING_HIGH:
				SetState(GOOMBA_STATE_JUMPING_HIGH);
				break;
			case GOOMBA_STATE_FALLING_LOW:
				SetState(GOOMBA_STATE_JUMPING_HIGH);
				break;
			default:
				break;
			}
			isTouchedAnotherGoomba = false;
		}
		else {
			//When red goomba is flying
			switch (this->state)
			{
			case GOOMBA_STATE_JUMPING_HIGH:
				if (this->start_Y - this->y >= GOOMBA_MAX_JUMP_HIGH_DY)
				{
					this->SetState(GOOMBA_STATE_FALLING_HIGH);
				}
				break;
			case GOOMBA_STATE_JUMPING_LOW:
				if (this->start_Y - this->y >= GOOMBA_MAX_JUMP_LOW_DY)
				{
					this->SetState(GOOMBA_STATE_FALLING_LOW);
				}
				break;
			case GOOMBA_STATE_JUMPING_AWAY_FROM_QUESTION_BRICK:
				if (this->touchedBrickTop - this->y >= GOOMBA_MAX_JUMP_HIGH_DY)
				{
					this->SetState(GOOMBA_STATE_FALLING_AWAY_FROM_QUESTION_BRICK);
				}
				break;
			default:
				break;
			}
		}
	}

}
void CGoomba::updateYellowGoomba(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	if (state != GOOMBA_STATE_DIE) {
		vy += ay * dt;
	}

	coEvents.clear();

	// turn off collision when die

	if (state != GOOMBA_STATE_DIE || state != GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS) {
		CalcPotentialCollisions(coObjects, coEvents);
	}

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

		float x0 = x, y0 = y;

		x = x0 + min_tx * dx + nx * PUSHBACK;
		y = y0 + min_ty * dy + ny * PUSHBACK;


		if (ny != 0) vy = 0;


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			float mLeft, mTop, mRight, mBottom;
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (dynamic_cast<CBrick*>(e->obj))
			{
				float oLeft, oTop, oRight, oBottom;
				CBrick* object = dynamic_cast<CBrick*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);

				if (e->ny < 0)
				{
					vy = GOOMBA_DIE_DEFLECT_SPEED;
					if (state != GOOMBA_STATE_FALLING_KILLED_BY_KOOPAS) {
						vy = 0;
					}
					else {
						y = y0 + dy;
						x = x0 + dx;
					}
					ay = GOOMBA_GRAVITY;
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
			if (dynamic_cast<CQuestionBrick*>(e->obj)) {
				float oLeft, oTop, oRight, oBottom;
				CQuestionBrick* object = dynamic_cast<CQuestionBrick*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);

				if (e->ny != 0)
				{
					vy = 0;
					ay = GOOMBA_GRAVITY;
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
			if (dynamic_cast<CGoomba*>(e->obj)) {
				//When goomba touches each other
				if (e->nx != 0) {
					x += dx;
				}
				else if (e->ny != 0) {
					y = start_Y;
					x += dx;
				}
			}
			if (dynamic_cast<CBlock*>(e->obj)) {
				if (e->ny != 0) {
					vy += ay * dt;
					y += dy;
					x += dx;
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny < 0 && koopas->GetState() == KOOPAS_STATE_SPINNING) {
					DebugOut(L"aaaaaaaaaaaaaaaaaa \n");
					ay = -ay;
					if (nx > 1)
						vx = GOOMBA_WALKING_SPEED;
					else
						vx = -GOOMBA_WALKING_SPEED;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CGoomba::updateRedGoomba(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != GOOMBA_STATE_RED_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);
	}

	if (state == GOOMBA_STATE_RED_WALKING) {
		vy += ay * dt;
	}
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
			if (dynamic_cast<CBrick*>(e->obj))
			{
				float oLeft, oTop, oRight, oBottom;
				CBrick* object = dynamic_cast<CBrick*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);

				if (e->ny != 0)
				{
					if (state == GOOMBA_STATE_FALLING_LOW || state == GOOMBA_STATE_FALLING_HIGH) {
						lowFallingTime++;
					}
					vy = 0;
					ay = GOOMBA_GRAVITY;
					isOnGround = true;
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
			else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
				float oLeft, oTop, oRight, oBottom;
				CQuestionBrick* object = dynamic_cast<CQuestionBrick*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (e->ny > 0)
				{
					isQuestionBrickAboveTouched = true;;
				}
				if (e->ny < 0) {
					isQuestionBrickBelowTouched = true;
					touchedBrickTop = oTop;
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
			else if (dynamic_cast<CGoomba*>(e->obj)) {
				//When goomba touches each other
				if (e->nx != 0) {
					x += dx;
				}
				if (e->ny != 0) {
					isTouchedAnotherGoomba = true;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CGoomba::Render()
{
	if (isDestroyed)
		return;
	int ani = -1;
	if (tag == GOOMBA_TAG_YELLOW) {
		ani = GOOMBA_ANI_YELLOW_WALKING;
		if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_ANI_YELLOW_DIE;
		}
	}
	else {
		ani = GOOMBA_ANI_RED_JUMPING;
		if (isOnGround)
		{
			//when red goomba is on the ground
			switch (state)
			{
			case GOOMBA_STATE_FALLING_LOW:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			case GOOMBA_STATE_FALLING_HIGH:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			case GOOMBA_STATE_FALLING_AWAY_FROM_QUESTION_BRICK:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			case GOOMBA_STATE_RED_WALK_WING:
				ani = GOOMBA_ANI_RED_WALK_WING;
				break;
			case GOOMBA_STATE_RED_WALKING:
				ani = GOOMBA_ANI_RED_WALK;
				break;
			case GOOMBA_STATE_RED_DIE:
				ani = GOOMBA_ANI_RED_DIE;
				break;
			default:
				break;
			}
		}
		else if (isQuestionBrickAboveTouched) {
			//when red goomba touches the question brick above its head
			switch (this->state)
			{
			case GOOMBA_STATE_JUMPING_HIGH:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			case GOOMBA_STATE_JUMPING_LOW:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			default:
				break;
			}
			isQuestionBrickAboveTouched = false;
		}
		else if (isQuestionBrickBelowTouched) {
			ani = GOOMBA_ANI_RED_JUMPING;
		}
		else if (isTouchedAnotherGoomba) {
			switch (this->state)
			{
			case GOOMBA_STATE_FALLING_HIGH:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			case GOOMBA_STATE_FALLING_LOW:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			default:
				break;
			}
		}
		else {
			//When red goomba is flying
			switch (this->state)
			{
			case GOOMBA_STATE_JUMPING_HIGH:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			case GOOMBA_STATE_JUMPING_LOW:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			case GOOMBA_STATE_JUMPING_AWAY_FROM_QUESTION_BRICK:
				ani = GOOMBA_ANI_RED_JUMPING;
				break;
			default:
				break;
			}
		}
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CGoomba::SetState(int state)
{
	CGameObject* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
	float mario_X, mario_Y;
	player->GetPosition(mario_X, mario_Y);

	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_NORMAL_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		this->transformToNonExistTimer.Start();
		break;
	case GOOMBA_STATE_YELLOW_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_FALLING_LOW:
		if (mario_X < this->x)
			vx = -GOOMBA_FALLING_LOW_SPEED_X;
		else
			vx = GOOMBA_FALLING_LOW_SPEED_X;
		vy = GOOMBA_FALLING_LOW_SPEED_Y;
		break;
	case GOOMBA_STATE_JUMPING_LOW:
		if (mario_X < this->x)
			vx = -GOOMBA_JUMPING_LOW_SPEED_X;
		else
			vx = GOOMBA_JUMPING_LOW_SPEED_X;
		vy = -GOOMBA_JUMPING_LOW_SPEED_Y;
		break;
	case GOOMBA_STATE_FALLING_HIGH:
		if (mario_X < this->x)
			vx = -GOOMBA_FALLING_HIGH_SPEED_X;
		else
			vx = GOOMBA_FALLING_HIGH_SPEED_X;
		vy = GOOMBA_FALLING_HIGH_SPEED_Y;
		break;
	case GOOMBA_STATE_JUMPING_HIGH:
		if (mario_X < this->x)
			vx = -GOOMBA_JUMPING_HIGH_SPEED_X;
		else
			vx = GOOMBA_JUMPING_HIGH_SPEED_X;
		vy = -GOOMBA_JUMPING_HIGH_SPEED_Y;
		break;
	case GOOMBA_STATE_RED_WALK_WING:
	{
		y += GOOMBA_BBOX_RED_UNWALK_HEIGHT - GOOMBA_BBOX_RED_WALK_WING_HEIGHT;
		redWalkWingTimer.Start();
		if (mario_X < this->x) {
			vx = -GOOMBA_WALKING_SPEED;
		}
		else {
			vx = GOOMBA_WALKING_SPEED;
		}
		ay = GOOMBA_GRAVITY;
		break;
	}
	case GOOMBA_STATE_RED_WALKING:
	{
		if (this->state != GOOMBA_STATE_RED_WALK_WING)
			y += GOOMBA_BBOX_RED_UNWALK_HEIGHT - GOOMBA_BBOX_NORMAL_HEIGHT;
		else
			y += GOOMBA_BBOX_RED_WALK_WING_HEIGHT - GOOMBA_BBOX_NORMAL_HEIGHT;
		if (!isOnGround)
			vy = GOOMBA_FALLING_HIGH_SPEED_Y;
		else
			vy = 0;
		vx = player->nx * GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		break;
	}
	case GOOMBA_STATE_RED_DIE:
		y += GOOMBA_BBOX_NORMAL_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		this->transformToNonExistTimer.Start();
		break;
	case GOOMBA_STATE_NON_EXIST:
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS:
		vy = -GOOMBA_JUMPING_KILLED_BY_KOOPAS_SPEED_Y;
		vx = -this->killingKoopasDirection * GOOMBA_JUMPING_KILLED_BY_KOOPAS_SPEED_X;
		ay = -GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_FALLING_KILLED_BY_KOOPAS:
		vy = GOOMBA_FALLING_KILLED_BY_KOOPAS_SPEED_Y;
		ay = GOOMBA_GRAVITY;
		vx = -this->killingKoopasDirection * GOOMBA_FALLING_KILLED_BY_KOOPAS_SPEED_X;
		break;
	case GOOMBA_STATE_JUMPING_AWAY_FROM_QUESTION_BRICK:
		vx = GOOMBA_JUMPING_LOW_SPEED_X;
		vy = -GOOMBA_JUMPING_LOW_SPEED_Y;
		break;
	case GOOMBA_STATE_FALLING_AWAY_FROM_QUESTION_BRICK:
		vx = GOOMBA_JUMPING_LOW_SPEED_X;
		vy = GOOMBA_JUMPING_LOW_SPEED_Y;
		break;
	}
	CGameObject::SetState(state);
}
