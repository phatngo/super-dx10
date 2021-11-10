
#include "FireBullet.h"
#include "FirePiranhaPlant.h"
#include "Camera.h"
#include "Game.h"

CFireBullet::CFireBullet(float x, float y, int FirePiranhaPlant_CurrentState)
{
	CGameObject::SetPosition(x, y);
	
	switch (FirePiranhaPlant_CurrentState)
	{
	case FIRE_PIRANHA_ANI_UP_RIGHT:
		this->SetState(FIRE_BULLET_STATE_UP_RIGHT);
		break;
	case FIRE_PIRANHA_ANI_UP_LEFT:
		this->SetState(FIRE_BULLET_STATE_UP_LEFT);
		break;
	case FIRE_PIRANHA_ANI_DOWN_RIGHT:
		this->SetState(FIRE_BULLET_STATE_DOWN_RIGHT);
		break;
	case FIRE_PIRANHA_ANI_DOWN_LEFT:
		this->SetState(FIRE_BULLET_STATE_DOWN_LEFT);
		break;
	default:
		break;
	}
}

void CFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroyed)
		return;
	left = x;
	top = y;
	right = x + FIRE_BULLET_BBOX_WIDTH;
	bottom = y + FIRE_BULLET_BBOX_HEIGHT;
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed)
		return;
	CGameObject::Update(dt, coObjects);
	y += dy;
	x += dx;

	float cameraPositionY = CCamera::GetInstance()->GetCameraY();
	float screenWidth = CGame::GetInstance()->GetScreenHeight();
	if (this->y < cameraPositionY || this->y > cameraPositionY + screenWidth) {
		isDestroyed = true;
	}
}

void CFireBullet::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(this->ani)->Render(x, y);
}

void CFireBullet::SetState(int state)
{
	switch (state)
	{
	case FIRE_BULLET_STATE_UP_RIGHT:
		this->SetPosition(x + FIRE_BULLET_BBOX_WIDTH, y + FIRE_BULLET_BBOX_HEIGHT);
		vx = FIRE_BULLET_X_VELOCITY;
		vy = -FIRE_BULLET_Y_VELOCITY;
		break;
	case FIRE_BULLET_STATE_UP_LEFT:
		vx = -FIRE_BULLET_X_VELOCITY;
		vy = -FIRE_BULLET_Y_VELOCITY;
		break;
	case FIRE_BULLET_STATE_DOWN_LEFT:
		vx = -FIRE_BULLET_X_VELOCITY;
		vy = FIRE_BULLET_Y_VELOCITY;
		break;
	case FIRE_BULLET_STATE_DOWN_RIGHT:
		this->SetPosition(x + FIRE_BULLET_BBOX_WIDTH, y + FIRE_BULLET_BBOX_HEIGHT);
		vx = FIRE_BULLET_X_VELOCITY;
		vy = FIRE_BULLET_Y_VELOCITY;
		break;
	case FIRE_BULLET_STATE_NON_EXIST:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}



