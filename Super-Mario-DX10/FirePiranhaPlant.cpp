#include "FirePiranhaPlant.h"
#include "Game.h"
#include "PlayScence.h"
#include "FireBullet.h"
#include "Utils.h"

CFirePiranhaPlant::CFirePiranhaPlant(int objTag)
{
	this->SetState(FIRE_PIRANHA_STATE_UP);
	if (objTag == TAG_FIRE_PIRANHA_RED)
		this->SetTag(TAG_FIRE_PIRANHA_RED);
	else
		this->SetTag(TAG_FIRE_PIRANHA_GREEN);
	isFireBulletCreated = false;
}

void CFirePiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIRE_PIRANHA_BBOX_WIDTH;
	bottom = y + FIRE_PIRANHA_BBOX_HEIGHT;
}

void CFirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;

	switch (state)
	{
	case FIRE_PIRANHA_STATE_UP:
	{
		switch (this->tag)
		{
		case TAG_FIRE_PIRANHA_RED:
			if (start_Y - y >= RED_FIRE_PIRANHA_HIGHEST_DY) {
				this->SetState(FIRE_PIRANHA_STATE_SHOOT);
				y = start_Y - RED_FIRE_PIRANHA_HIGHEST_DY;
			}
			break;
		case TAG_FIRE_PIRANHA_GREEN:
			if (start_Y - y >= GREEN_FIRE_PIRANHA_HIGHEST_DY) {
				this->SetState(FIRE_PIRANHA_STATE_SHOOT);
				y = start_Y - GREEN_FIRE_PIRANHA_HIGHEST_DY;
			}
			break;
		default:
			break;
		}
		break;
	}
	case FIRE_PIRANHA_STATE_SHOOT:
		if (shoot_Timer.ElapsedTime() >= SHOOT_DELAY_TIME && shoot_Timer.IsStarted()) {
			CreateFireBullet();
			shoot_Timer.Reset();
			down_Timer.Start();
		}
		else if (down_Timer.ElapsedTime() >= DOWN_DELAY_TIME && down_Timer.IsStarted()) {
			this->SetState(FIRE_PIRANHA_STATE_DOWN);
			down_Timer.Reset();
		}
		break;
	case FIRE_PIRANHA_STATE_DOWN:
		if (start_Y <= y) {
			if (up_Timer.ElapsedTime() >= UP_DELAY_TIME && up_Timer.IsStarted()) {
				this->SetState(FIRE_PIRANHA_STATE_UP);
				up_Timer.Reset();
			}
			else {
				y = start_Y;
			}
		}
		break;
	default:
		break;
	}
}

void CFirePiranhaPlant::Render()
{
	CGameObject* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();

	if (player == NULL)
		return;
	float mario_X, mario_Y;
	player->GetPosition(mario_X, mario_Y);
	switch (state)
	{
	case FIRE_PIRANHA_STATE_UP:
	{
		if (mario_X < this->x) {

			if (mario_Y <= this->y) {
				this->ani = FIRE_PIRANHA_ANI_UP_LEFT;
			}
			else {
				this->ani = FIRE_PIRANHA_ANI_DOWN_LEFT;
			}

		}
		else {

			if (mario_Y <= this->y) {
				this->ani = FIRE_PIRANHA_ANI_UP_RIGHT;
			}
			else {
				this->ani = FIRE_PIRANHA_ANI_DOWN_RIGHT;
			}
		}
		break;
	}
	default:
		break;
	}

	animation_set->at(this->ani)->Render(x, y + EXTRA_DY_MOVING_UP);
}

void CFirePiranhaPlant::SetState(int state)
{
	switch (state) {
	case FIRE_PIRANHA_STATE_UP:
		vy = -FIRE_PIRANHA_UP_SPEED;
		break;
	case FIRE_PIRANHA_STATE_DOWN:
		vy = FIRE_PIRANHA_UP_SPEED;
		up_Timer.Start();
		
		isFireBulletCreated = false;
		break;
	case FIRE_PIRANHA_STATE_SHOOT:
		vy = 0;
		shoot_Timer.Start();
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CFirePiranhaPlant::CreateFireBullet() {
	if (!isFireBulletCreated) {
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET tmp_ani_set = animation_sets->Get(FIRE_BULLET_ANI_SET_ID);
		CFireBullet* bullet = new CFireBullet(this->x, this->y, this->ani);
		bullet->SetAnimationSet(tmp_ani_set);
		scene->GetUnit()->AddUnit(bullet, scene->GetGrid());
		isFireBulletCreated = true;
	}
}


