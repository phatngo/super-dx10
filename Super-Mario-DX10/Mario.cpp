#include <algorithm>
#include <assert.h>

#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "Block.h"
#include "Coin.h"
#include "Brick.h"
#include "PiranhaPlant.h"
#include "FirePiranhaPlant.h"
#include "FireBullet.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "EffectPoint.h"
#include "Game.h"
#include "PlayScence.h"
#include "FlashAnimationBrick.h"
#include "Card.h"
#include "Scence.h"
#include "Sprites.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	totalPoint = 0;
	totalMoney = 0;
	isKickingKoopas = false;
	isOnGround = true;
	isChangeDirection = false;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> *objects)
{
	if (isPipedUp) {
		if (start_Y - y >= MARIO_DY_GET_OUT_FROM_PIPE) {
			//isPipedDown = false;
			isPipedUp = false;
		}
	}

	
	
	if (kickTimer.IsStarted() && kickTimer.ElapsedTime() >= KICK_TIME && isKickingKoopas) {
		isKickingKoopas = false;
		kickTimer.Reset();
	}
	
	if (!isHold) {
		vx += ax * dt;
		if (CGame::GetInstance()->IsKeyDown(DIK_Z) && (CGame::GetInstance()->IsKeyDown(DIK_RIGHT) || (CGame::GetInstance()->IsKeyDown(DIK_LEFT))) && abs(vx) >= MARIO_RUNNING_SPEED_MAX) {
			if (vx >= MARIO_SPEED_RUN_FLY_MAX) {
				vx = MARIO_SPEED_RUN_FLY_MAX;
			}
			else if (vx <= -MARIO_SPEED_RUN_FLY_MAX) {
				vx = -MARIO_SPEED_RUN_FLY_MAX;
			}
		}
		else {
			if (vx >= MARIO_RUNNING_SPEED_MAX) {
				vx = MARIO_RUNNING_SPEED_MAX;
			}
			else if (vx <= -MARIO_RUNNING_SPEED_MAX) {
				vx = -MARIO_RUNNING_SPEED_MAX;
			}
		}
	}

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (abs(vx) == MARIO_SPEED_RUN_FLY_MAX) {
		isFlyingToTheSky = true;
		vy += 0;
	}
	else {
		vy += MARIO_GRAVITY * dt;
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{	
		if (x <= 0) {
			x = 0;
		}
		else if (x >= maxXcoordinate && !CGame::GetInstance()->GetCurrentScene()->GetSceneDone()) {
			x = maxXcoordinate;
		}
		else {
			x += dx;
		}
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		//DebugOut(L"[INFO] e1 is goomba: %d \n", dynamic_cast<CGoomba*>(e1->obj) ? 1 : 0);
		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		/*if (rdx != 0 && rdx != dx)
			x += nx*abs(rdx);*/ 
		// block every object first!
		float y0 = y;
		float x0 = x;

		x = x0 + min_tx * dx + nx * PUSHBACK;
		y = y0 + min_ty * dy + ny * PUSHBACK;
		
		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			float oLeft, oTop, oRight, oBottom;			
			float mLeft, mTop, mRight, mBottom;
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{

				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0 && goomba->GetState() != GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS)
				{
					if(goomba->GetTag()==GOOMBA_TAG_YELLOW){
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
								goomba->SetState(GOOMBA_STATE_DIE);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else {
						if (goomba->GetState() != GOOMBA_STATE_RED_WALKING && goomba->GetState()!= GOOMBA_STATE_RED_DIE) {
							this->AddPoint(oLeft, oTop - GOOMBA_BBOX_NORMAL_HEIGHT, EFFECT_POINT_400);
							goomba->SetState(GOOMBA_STATE_RED_WALKING);
						}
						else if (goomba->GetState() == GOOMBA_STATE_RED_WALKING) {
							 goomba->SetState(GOOMBA_STATE_RED_DIE);
						}
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->ny < 0 && goomba->GetState() == GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS) {
					goomba->SetState(GOOMBA_STATE_FALLING_KILLED_BY_KOOPAS);
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								if (level == MARIO_LEVEL_TAIL) {
									level = MARIO_LEVEL_TRANSFORM_BIG;
									this->transformTimer.Start();
								}
								else {
									level = MARIO_LEVEL_TRANSFORM_SMALL;
									this->transformTimer.Start();
								}
								    StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
		    if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (koopas->GetTag() == KOOPAS_RED || koopas->GetTag() == KOOPAS_GREEN) {
					if (e->ny < 0)
					{
						if (koopas->GetState() != KOOPAS_STATE_DEATH)
						{
							if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
								koopas->SetState(KOOPAS_STATE_SPINNING);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
								AddPoint(oLeft, oTop - KOOPAS_BBOX_SHELL_HEIGHT);
							}
							else {
								koopas->SetState(KOOPAS_STATE_IN_SHELL);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
								AddPoint(oLeft, oTop - KOOPAS_BBOX_SHELL_HEIGHT, EFFECT_POINT_200);
							}
						}
					}
					else if (e->nx != 0)
					{
						//If mario is touchable
						if (untouchable == 0)
						{
							//If koopas is not dead
							if (koopas->GetState() == KOOPAS_STATE_IN_SHELL)
							{
								isReadyToHold = true;
								if (state != MARIO_STATE_DIE) {
									if (isReadyToHold && CGame::GetInstance()->IsKeyDown(DIK_Q)) {
										koopas->SetIsHold(true);
									}
									else {
										isKickingKoopas = true;
										koopas->SetState(KOOPAS_STATE_SPINNING);
										kickTimer.Start();
									}
								}
							}
							else if (koopas->GetState() == KOOPAS_STATE_SPINNING) {
								if (!isKickingKoopas) {
									if (level > MARIO_LEVEL_SMALL)
									{
										if (level == MARIO_LEVEL_TAIL) {
											level = MARIO_LEVEL_TRANSFORM_BIG;
											this->transformTimer.Start();
										}
										else {
											level = MARIO_LEVEL_TRANSFORM_SMALL;
											this->transformTimer.Start();
										}
										StartUntouchable();
									}
									else
										//Makes mario die
										SetState(MARIO_STATE_DIE);
								}
							}
							else {
								if (level > MARIO_LEVEL_SMALL)
								{
									if (level == MARIO_LEVEL_TAIL) {
										level = MARIO_LEVEL_TRANSFORM_BIG;
										this->transformTimer.Start();
									}
									else {
										level = MARIO_LEVEL_TRANSFORM_SMALL;
										this->transformTimer.Start();
									}
									StartUntouchable();
								}
								else
									//Makes mario die
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				else {
					if (e->nx != 0) {
						if (untouchable == 0) {
							if (level > MARIO_LEVEL_SMALL)
							{
								if (level == MARIO_LEVEL_TAIL) {
									level = MARIO_LEVEL_TRANSFORM_BIG;
									this->transformTimer.Start();
								}
								else {
									level = MARIO_LEVEL_TRANSFORM_SMALL;
									this->transformTimer.Start();
								}
								StartUntouchable();
							}
							else
								//Makes mario die
								SetState(MARIO_STATE_DIE);
						}
					}
					if (e->ny != 0) {
						if (koopas->GetState() != KOOPAS_STATE_DEATH)
						{
							if (e->ny < 0) {
								koopas->SetTag(KOOPAS_GREEN);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
								AddPoint(oLeft, oTop - KOOPAS_BBOX_SHELL_HEIGHT);
							}
							else {
								if (untouchable == 0) {
									if (level > MARIO_LEVEL_SMALL)
									{
										if (level == MARIO_LEVEL_TAIL) {
											level = MARIO_LEVEL_TRANSFORM_BIG;
											this->transformTimer.Start();
										}
										else {
											level = MARIO_LEVEL_TRANSFORM_SMALL;
											this->transformTimer.Start();
										}
										StartUntouchable();
									}
									else
										//Makes mario die
										SetState(MARIO_STATE_DIE);
								}
							}
						}
					}
				}
			}
			if (dynamic_cast<CBlock*>(e->obj)) 
			{
				CBlock* block = dynamic_cast<CBlock*>(e->obj);
				float bTop, bLeft, bRight, bBottom;
				block->GetBoundingBox(bLeft, bTop, bRight, bBottom);
				if (e->nx != 0 && ceil(mBottom) != oTop) {
					vx += ax * dt;
					x = x0 + dx;
				}
				if (e->ny < 0) {
					vy = 0;
					isOnGround = true;
					if (state == MARIO_STATE_RELEASE_JUMP)
						SetState(MARIO_STATE_IDLE);
				}
				if (e->ny > 0) 
				{
					y += dy;
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}
			if (dynamic_cast<CQuestionBrick*>(e->obj)) // if e->obj is Quesion Brick
			{
				CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
				//Mario stays under question brick
				if (e->ny > 0)
				{
					if(e->obj->GetState()!= QUESTION_BRICK_STATE_STOP)
						questionBrick->SetState(QUESTION_BRICK_STATE_JUMPING);	
				}
				else if (e->ny < 0) {
					isOnGround = true;
					if (state == MARIO_STATE_RELEASE_JUMP)
						SetState(MARIO_STATE_IDLE);
				}
			}
			if (dynamic_cast<CBrick*>(e->obj)) {
			CBrick* brick = dynamic_cast<CBrick*>(e->obj);
			if (e->ny < 0) {
				isOnGround = true;
				if (brick->IsPipe() && brick->IsAbleToGoIn()) {
					
				}
				else {
					if (CGame::GetInstance()->GetCurrentScene()->GetSceneDone()) {
						vx = MARIO_WALKING_SPEED_MAX;
					}
				}
				}
			}
			if (dynamic_cast<CFlashAnimationBrick*>(e->obj)) {
			if (e->ny < 0) {
				isOnGround = true;
				if (state == MARIO_STATE_RELEASE_JUMP)
					SetState(MARIO_STATE_IDLE);
			}
            }
			if (dynamic_cast<CCoin*>(e->obj))
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				if (e->ny > 0) {
					vy = -MARIO_JUMP_SPEED_Y;
				}
				if (e->nx != 0) {
					x = x0 + dx;
				}
				coin->SetIsDestroyed();
				AddMoney();
			}
			if (dynamic_cast<CFireBullet*>(e->obj)) {
				if (untouchable == 0)
				{
						//If mario is not in the big size
					if (level > MARIO_LEVEL_SMALL)
					{
						if (level == MARIO_LEVEL_TAIL) {
							level = MARIO_LEVEL_TRANSFORM_BIG;
							this->transformTimer.Start();
						}
						else if(level == MARIO_LEVEL_BIG) {
							level = MARIO_LEVEL_TRANSFORM_SMALL;
							this->transformTimer.Start();
						}
						StartUntouchable();
					}
						else
							//Makes mario die
							SetState(MARIO_STATE_DIE);	
				}
            }
			if (dynamic_cast<CPiranhaPlant*>(e->obj)) {
			if (untouchable == 0)
			 {
				CPiranhaPlant* piranhaPlant = dynamic_cast<CPiranhaPlant*>(e->obj);
				if (e->ny < 0 && piranhaPlant->GetState()== PIRANHA_STATE_UP) {
					piranhaPlant->SetState(PIRANHA_STATE_DOWN);

				}
				//If mario is not in the big size
				else if (level > MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_TAIL) {
						level = MARIO_LEVEL_TRANSFORM_BIG;
						this->transformTimer.Start();
					}
					else if (level == MARIO_LEVEL_BIG) {
						level = MARIO_LEVEL_TRANSFORM_SMALL;
						this->transformTimer.Start();
					}
					StartUntouchable();
				}
				else
					//Makes mario die
					SetState(MARIO_STATE_DIE);
			 }
            }
			if (dynamic_cast<CFirePiranhaPlant*>(e->obj)) {
			if (untouchable == 0)
			{
				CFirePiranhaPlant* firePiranhaPlant = dynamic_cast<CFirePiranhaPlant*>(e->obj);
				if (e->ny < 0 && firePiranhaPlant->GetState() == FIRE_PIRANHA_STATE_UP ) {
					firePiranhaPlant->SetState(FIRE_PIRANHA_STATE_DOWN);
				}
				//If mario is not in the big size
				else if (level > MARIO_LEVEL_SMALL)
				{
					if (level == MARIO_LEVEL_TAIL) {
						level = MARIO_LEVEL_TRANSFORM_BIG;
						this->transformTimer.Start();
					}
					else if (level == MARIO_LEVEL_BIG) {
						level = MARIO_LEVEL_TRANSFORM_SMALL;
						this->transformTimer.Start();
					}
					StartUntouchable();
				}
				else
					//Makes mario die
					SetState(MARIO_STATE_DIE);
			}
			}
			if (dynamic_cast<CMushroom*>(e->obj)) 
            {
			CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
			mushroom->SetIsDestroyed();
			if (e->ny > 0) {
				//Change the rendering y to the y of big mario
				y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
			}
			if (this->level < MARIO_LEVEL_BIG) {
				//Change the rendering y to the y of big mario
				y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
				this->level = MARIO_LEVEL_TRANSFORM_BIG;
				this->transformTimer.Start();
				AddPoint(this->x, this->y, EFFECT_POINT_1000);
			}

			}
			if (dynamic_cast<CLeaf*>(e->obj)) {
			CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
			leaf->SetIsDestroyed();
			if (e->ny > 0) {
				//Change the rendering y to the y of big mario
				if (this->level == MARIO_LEVEL_SMALL) {
					y -= (MARIO_TAIL_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
				}
				else {
					y -= (MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
				}
			}
			if (this->level < MARIO_LEVEL_TAIL) 
			{
				if (this->level == MARIO_LEVEL_SMALL) {
					y -= (MARIO_TAIL_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
				}
				else {
					y -= (MARIO_TAIL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
				}
				this->level = MARIO_LEVEL_TRANSFORM_TAIL;
				this->transformTimer.Start();
			}
			}
			if (dynamic_cast<Card*>(e->obj)) {
			Card* card = dynamic_cast<Card*>(e->obj);
			if (e->ny > 0) {
				card->SetState(CARD_STATE_FLY_UP);
				CGame::GetInstance()->GetCurrentScene()->SetSceneDone();
			}
            }
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	

	if (level != MARIO_LEVEL_SMALL && level != MARIO_LEVEL_TRANSFORM_SMALL && state != MARIO_STATE_DIE && !isSitDown)
		postion_y = y + MARIO_TRANSFORM_DY_FROM_SMALL_TO_BIG;
	else
		postion_y = y;

	if (isSitDown && level == MARIO_LEVEL_BIG && vx == 0) {
		postion_y = y + (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_BBOX_SIT_HEIGHT) + MARIO_SIT_DOWN_DY;

	}

	if (isSitDown && level == MARIO_LEVEL_TAIL && vx == 0) {
			postion_y = y + (MARIO_TAIL_BBOX_HEIGHT - MARIO_TAIL_BBOX_SIT_HEIGHT) + MARIO_SIT_DOWN_DY;
	}

	if (isPipedDown) {
		vy = MARIO_PIPE_SPEED;
		y += vy * dt;
	}
	if (isPipedUp) {
		vy = -MARIO_PIPE_SPEED;
		y += vy * dt;
	}

	

}

void CMario::Render()
{
	float alpha = ALPHA;
	if (untouchable) alpha = UNTOUCHABLE_ALPHA;
	ani = -1;
	if (!isPipedUp && !isPipedDown) {
		if (state == MARIO_STATE_DIE)
		{
			ani = MARIO_ANI_DIE;
		}
		else {
			if (level == MARIO_LEVEL_TRANSFORM_BIG)
			{
				if (nx > 0) {
					ani = MARIO_ANI_TRANSFORM_BIG_RIGHT;
				}
				else {
					ani = MARIO_ANI_TRANSFORM_BIG_LEFT;
				}
				if (transformTimer.ElapsedTime() >= MARIO_TRANSFORMING_TIME && transformTimer.IsStarted()) {
					level = MARIO_LEVEL_BIG;
					transformTimer.Reset();
				}
			}
			else if (level == MARIO_LEVEL_TRANSFORM_SMALL)
			{
				if (nx > 0) {
					ani = MARIO_ANI_TRANSFORM_BIG_RIGHT;
				}
				else {
					ani = MARIO_ANI_TRANSFORM_BIG_LEFT;
				}
				if (transformTimer.ElapsedTime() >= MARIO_TRANSFORMING_TIME && transformTimer.IsStarted()) {
					level = MARIO_LEVEL_SMALL;
					transformTimer.Reset();
				}
			}
			else if (level == MARIO_LEVEL_TRANSFORM_TAIL)
			{
				ani = MARIO_ANI_TRANSFORM_TAIL;
				if (transformTimer.ElapsedTime() >= MARIO_TRANSFORMING_TIME && transformTimer.IsStarted()) {
					level = MARIO_LEVEL_TAIL;
					transformTimer.Reset();
					AddPoint(this->x, this->y, EFFECT_POINT_1000);
				}
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (isHold)
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							if (vy < 0)
								ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
							else
								ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
						}
						else {
							if (vy < 0)
								ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
							else
								ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
						}
					}
					else if (vx > 0)
					{
						if (vy < 0)
							ani = MARIO_ANI_TAIL_HOLD_JUMP_RIGHT;
						else
							ani = MARIO_ANI_TAIL_HOLD_WALK_RIGHT;
					}
					else
					{
						if (vy < 0)
							ani = MARIO_ANI_TAIL_HOLD_JUMP_LEFT;
						else
							ani = MARIO_ANI_TAIL_HOLD_WALK_LEFT;
					}
				}
				else if (vy < 0) {
					if (nx > 0) {
						if (vx == MARIO_SPEED_RUN_FLY_MAX) {
							ani = MARIO_ANI_TAIL_FLY_UP_RIGHT;
						}
						else
							ani = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
					}
					else {
						if (abs(vx) == MARIO_SPEED_RUN_FLY_MAX) {
							ani = MARIO_ANI_TAIL_FLY_UP_LEFT;
						}
						else
							ani = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
					}
				}
				else if (vx == 0)
				{
					if (nx > 0) {
						if (vy < 0) {
							if (state == MARIO_STATE_JUMP)
								ani = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
							else
								ani = MARIO_ANI_TAIL_JUMPINGDOWN_RIGHT;
						}
						else if (state == MARIO_STATE_SIT) {
							ani = MARIO_ANI_TAIL_SIT_LEFT;
						}
						else
							ani = MARIO_ANI_TAIL_IDLE_RIGHT;
					}
					else {
						if (vy < 0) {
							if (state == MARIO_STATE_JUMP)
								ani = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
							else
								ani = MARIO_ANI_TAIL_JUMPINGDOWN_LEFT;
						}
						if (state == MARIO_STATE_SIT) {
							ani = MARIO_ANI_TAIL_SIT_RIGHT;
						}
						else
							ani = MARIO_ANI_TAIL_IDLE_LEFT;
					}
				}
				else if (vx > 0) {
					if (isKickingKoopas) {
						ani = MARIO_ANI_TAIL_KICK_RIGHT;
					}
					else {
						if (ax < 0 && nx < 0) {
							ani = MARIO_ANI_TAIL_BRAKING_LEFT;
						}
						else {
							if (vx < MARIO_WALKING_SPEED_MAX) {
								ani = MARIO_ANI_TAIL_WALKING_RIGHT;
							}
							else if (vx >= MARIO_WALKING_SPEED_MAX && vx < MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_TAIL_WALKING_FAST_RIGHT;
							}
							else if (vx >= MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_TAIL_MAX_SPEED_RIGHT;
							}
						}
					}
				}
				else {
					if (isKickingKoopas) {
						ani = MARIO_ANI_TAIL_KICK_LEFT;
					}
					else {
						if (ax > 0 && nx > 0)
							ani = MARIO_ANI_TAIL_BRAKING_RIGHT;
						else {
							if (abs(vx) < MARIO_WALKING_SPEED_MAX) {
								ani = MARIO_ANI_TAIL_WALKING_LEFT;
							}
							else if (abs(vx) >= MARIO_WALKING_SPEED_MAX && abs(vx) < MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_TAIL_WALKING_FAST_LEFT;
							}
							else if (abs(vx) >= MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_TAIL_MAX_SPEED_LEFT;
							}
						}
					}
				}
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				if (isHold)
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							if (vy < 0)
								ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
							else
								ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
						}
						else {
							if (vy < 0)
								ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
							else
								ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
						}
					}
					else if (vx > 0)
					{
						if (vy < 0)
							ani = MARIO_ANI_BIG_HOLD_JUMP_RIGHT;
						else
							ani = MARIO_ANI_BIG_HOLD_WALK_RIGHT;
					}
					else
					{
						if (vy < 0)
							ani = MARIO_ANI_BIG_HOLD_JUMP_LEFT;
						else
							ani = MARIO_ANI_BIG_HOLD_WALK_LEFT;
					}
				}
				else if (vy < 0) {
					if (nx > 0) {
						if (vx == MARIO_SPEED_RUN_FLY_MAX) {
							ani = MARIO_ANI_BIG_FLY_RIGHT;
						}
						else
							ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
					}
					if (nx < 0) {
						if (abs(vx) == MARIO_SPEED_RUN_FLY_MAX) {
							ani = MARIO_ANI_BIG_FLY_LEFT;
						}
						else
							ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
					}
				}
				else if (vx == 0)
				{
					if (nx > 0)
					{
						if (vy < 0) {
							if (state == MARIO_STATE_JUMP)
								ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
							else
								ani = MARIO_ANI_BIG_JUMPINGDOWN_RIGHT;
						}
						else if (state == MARIO_STATE_SIT) {
							ani = MARIO_ANI_BIG_SIT_RIGHT;
						}
						else
							ani = MARIO_ANI_BIG_IDLE_RIGHT;
					}
					else
					{
						if (vy < 0) {
							if (state == MARIO_STATE_JUMP)
								ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
							else
								ani = MARIO_ANI_BIG_JUMPINGDOWN_LEFT;
						}
						else if (state == MARIO_STATE_SIT) {
							ani = MARIO_ANI_BIG_SIT_LEFT;
						}
						else
							ani = MARIO_ANI_BIG_IDLE_LEFT;
					}
				}
				else if (vx > 0)
				{
					if (isKickingKoopas)
					{
						ani = MARIO_ANI_BIG_KICK_RIGHT;
					}
					else
					{
						if (ax < 0 && nx < 0) {
							ani = MARIO_ANI_BIG_BRAKING_LEFT;
						}
						else if (this->state == MARIO_STATE_SIT) {
							ani = MARIO_ANI_BIG_SIT_RIGHT;
						}
						else {
							if (vx < MARIO_WALKING_SPEED_MAX) {
								ani = MARIO_ANI_BIG_WALKING_RIGHT;
							}
							else if (vx >= MARIO_WALKING_SPEED_MAX && vx < MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
							}
							else if (vx >= MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_BIG_MAX_SPEED_RIGHT;
							}
						}
					}
				}
				else {
					if (isKickingKoopas) {
						ani = MARIO_ANI_BIG_KICK_LEFT;
					}
					else {
						if (ax > 0 && nx > 0) {
							ani = MARIO_ANI_BIG_BRAKING_RIGHT;
						}
						else if (this->state == MARIO_STATE_SIT) {
							ani = MARIO_ANI_BIG_SIT_LEFT;
						}
						else {
							if (abs(vx) < MARIO_WALKING_SPEED_MAX) {
								ani = MARIO_ANI_BIG_WALKING_LEFT;
							}
							else if (abs(vx) >= MARIO_WALKING_SPEED_MAX && abs(vx) < MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_BIG_WALKING_FAST_LEFT;
							}
							else if (abs(vx) >= MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_BIG_MAX_SPEED_LEFT;
							}
						}
					}
				}
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (isHold)
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							if (vy < 0)
								ani = MARIO_ANI_SMALL_HOLD_JUMP_RIGHT;
							else
								ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
						}
						else {
							if (vy < 0)
								ani = MARIO_ANI_SMALL_HOLD_JUMP_LEFT;
							else
								ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
						}
					}
					else if (vx > 0)
					{
						if (vy < 0)
							ani = MARIO_ANI_SMALL_HOLD_JUMP_RIGHT;
						else
							ani = MARIO_ANI_SMALL_HOLD_WALK_RIGHT;
					}
					else
					{
						if (vy < 0)
							ani = MARIO_ANI_SMALL_HOLD_JUMP_LEFT;
						else
							ani = MARIO_ANI_SMALL_HOLD_WALK_LEFT;
					}
				}
				else if (vx == 0)
				{
					if (nx > 0)
					{
						if (vy < 0) {
							if (state == MARIO_STATE_JUMP)
								ani = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
							else
								ani = MARIO_ANI_SMALL_JUMPINGDOWN_RIGHT;
						}
						else
							ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					}
					else {
						if (vy < 0) {
							if (state == MARIO_STATE_JUMP)
								ani = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
							else
								ani = MARIO_ANI_SMALL_JUMPINGDOWN_LEFT;
						}
						else
							ani = MARIO_ANI_SMALL_IDLE_LEFT;
					}
				}
				else if (vx > 0) {
					if (isKickingKoopas) {
						ani = MARIO_ANI_SMALL_KICK_RIGHT;
					}
					else {
						if (vy < 0) {
							if (vx == MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_SMALL_FLY_RIGHT;
							}
							else if (state == MARIO_STATE_JUMP) {
								ani = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
							}
							else
								ani = MARIO_ANI_SMALL_JUMPINGDOWN_RIGHT;
						}
						else if (ax < 0 && nx < 0) {
							ani = MARIO_ANI_SMALL_BRAKING_LEFT;
						}
						else {
							if (vx < MARIO_WALKING_SPEED_MAX) {
								ani = MARIO_ANI_SMALL_WALKING_RIGHT;
							}
							else if (vx >= MARIO_WALKING_SPEED_MAX && vx < MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_SMALL_WALKING_FAST_RIGHT;
							}
							else if (vx >= MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_SMALL_MAX_SPEED_RIGHT;
							}
						}
					}
				}
				else {
					if (isKickingKoopas) {
						ani = MARIO_ANI_SMALL_KICK_LEFT;
					}
					else {
						if (vy < 0) {
							if (abs(vx) == MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_SMALL_FLY_LEFT;
							}
							else if (state == MARIO_STATE_JUMP) {
								if (abs(vx) == MARIO_SPEED_RUN_FLY_MAX)
									ani = MARIO_ANI_SMALL_FLY_LEFT;
								else
									ani = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
							}
							else
								ani = MARIO_ANI_SMALL_JUMPINGDOWN_LEFT;
						}
						else if (ax > 0 && nx > 0) {
							ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
						}
						else {
							if (abs(vx) < MARIO_WALKING_SPEED_MAX) {
								ani = MARIO_ANI_SMALL_WALKING_LEFT;
							}
							else if (abs(vx) >= MARIO_WALKING_SPEED_MAX && abs(vx) < MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_SMALL_WALKING_FAST_LEFT;
							}
							else if (abs(vx) >= MARIO_SPEED_RUN_FLY_MAX) {
								ani = MARIO_ANI_SMALL_MAX_SPEED_LEFT;
							}
						}
					}
				}
			}

		}
		animation_set->at(ani)->Render(x, postion_y, alpha);
	}
	if (isPipedDown) {
		if (level == MARIO_LEVEL_SMALL) {
			CSprites::GetInstance()->Get(MARIO_SPRITE_PIPING_SMALL)->Draw(x, y);
		}
		else if (level == MARIO_LEVEL_BIG) {
			CSprites::GetInstance()->Get(MARIO_SPRITE_PIPING_BIG)->Draw(x, y);
		}else if (level == MARIO_LEVEL_TAIL) {
			CSprites::GetInstance()->Get(MARIO_SPRITE_PIPING_TAIL)->Draw(x, y);
		}
	}
	if (isPipedUp) {
		if (level == MARIO_LEVEL_SMALL) {
			CSprites::GetInstance()->Get(MARIO_SPRITE_PIPING_SMALL)->Draw(x, y);
		}
		else if (level == MARIO_LEVEL_BIG) {
			CSprites::GetInstance()->Get(MARIO_SPRITE_PIPING_BIG)->Draw(x, y);
		}
		else if (level == MARIO_LEVEL_TAIL) {
			CSprites::GetInstance()->Get(MARIO_SPRITE_PIPING_TAIL)->Draw(x, y);
		}
	}
	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (!isHold) {
			ax = MARIO_ACCEL_WALK_X;
		}
		else {
			vx = MARIO_WALKING_SPEED_MAX;
		}
		nx = 1;
		if (this->state == MARIO_STATE_SIT) {
			isSitDown = false;
		}
		isFlyingToTheSky = false;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		if (!isHold) {
			ax = -MARIO_ACCEL_WALK_X;
		}
		else {
			vx = -MARIO_WALKING_SPEED_MAX;
		}
		nx = -1;
		if (this->state == MARIO_STATE_SIT) {
			isSitDown = false;
		}
		isFlyingToTheSky = false;
		break;
	case MARIO_STATE_JUMP:
		if (isOnGround && !isReadyToRunMax) {
			vy = -MARIO_JUMP_SPEED_Y;
			isOnGround = false;
		}
		if (isOnGround && isReadyToRunMax) {
			isFlyingToTheSky = true;
			vy = -MARIO_JUMP_SPEED_Y;
			ax = MARIO_ACCELERATION;
		}
		break; 
	case MARIO_STATE_IDLE:
		if (!isHold) {
			if (vx > 0)
				ax = -MARIO_ACCELERATION;
			if (vx < 0)
				ax = MARIO_ACCELERATION;
		}
		else {
			vx = 0;
		}
		ay = MARIO_GRAVITY;
		if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
		{
			vx = 0;
			ax = 0;
		}
		if (this->state == MARIO_STATE_SIT) {
			isSitDown = false;
		}
		isFlyingToTheSky = false;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		isFlyingToTheSky = false;
		break;
	case MARIO_STATE_SIT:
		if (level != MARIO_LEVEL_SMALL) {
			isSitDown = true;
			vx = 0;
			ax = 0;
		}
		isFlyingToTheSky = false;
		break;
	}
	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG 
	|| level==MARIO_LEVEL_TRANSFORM_BIG 
	|| level==MARIO_LEVEL_TRANSFORM_SMALL
	|| (level == MARIO_LEVEL_BIG && isHold))
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL || level==MARIO_LEVEL_TRANSFORM_TAIL || (level == MARIO_LEVEL_TAIL && isHold))
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
	}
	else if (isSitDown) {
		if (level == MARIO_LEVEL_BIG) {
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_SIT_HEIGHT;
		}
		else if (level == MARIO_LEVEL_TAIL) {
			right = x + MARIO_TAIL_BBOX_SIT_WIDTH;
			bottom = y + MARIO_TAIL_BBOX_SIT_HEIGHT;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::AddPoint(float x, float y, int point) {
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET tmp_ani_set = animation_sets->Get(ANI_SET_ID_POINT_100);
	switch (point)
	{
	case EFFECT_POINT_200:
		tmp_ani_set = animation_sets->Get(ANI_SET_ID_POINT_200);
		break;
	case EFFECT_POINT_400:
		tmp_ani_set = animation_sets->Get(ANI_SET_ID_POINT_400);
		break;
	case EFFECT_POINT_800:
		tmp_ani_set = animation_sets->Get(ANI_SET_ID_POINT_800);
		break;
	case EFFECT_POINT_1000:
		tmp_ani_set = animation_sets->Get(ANI_SET_ID_POINT_1000);
		break;
	default:
		break;
	}
	this->totalPoint += point;
	EffectPoint* effectPoint = new EffectPoint();
	effectPoint->SetPosition(x, y);
	effectPoint->SetAnimationSet(tmp_ani_set);
	scene->GetUnit()->AddUnit(effectPoint, scene->GetGrid());
}

void CMario::SetIsPipe(int pipeDirection) {
	if (pipeDirection < 0) {
		isPipedUp = true;
		isPipedDown = false;
		pipeTimer.Start();
	}
	else if (pipeDirection > 0) {
		isPipedDown = true;
		isPipedUp = false;
		pipeTimer.Start();
	}
	else {
			isPipedUp = false;
			isPipedDown = false;
			pipeTimer.Reset();
	}
}
