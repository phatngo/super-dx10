#pragma once
#include "GameObject.h"

#define FIRE_BULLET_BBOX_WIDTH 8
#define FIRE_BULLET_BBOX_HEIGHT 8

#define FIRE_BULLET_X_VELOCITY 0.05f
#define FIRE_BULLET_Y_VELOCITY 0.05f

#define FIRE_BULLET_STATE_UP_LEFT    100
#define FIRE_BULLET_STATE_DOWN_LEFT  200
#define FIRE_BULLET_STATE_UP_RIGHT   300
#define FIRE_BULLET_STATE_DOWN_RIGHT 400
#define FIRE_BULLET_STATE_NON_EXIST  500

#define FIRE_BULLET_ANI_SET_ID 9
#define FIRE_BULLET_ANI_RIGHT 0
#define FIRE_BULLET_ANI_LEFT 1


class CFireBullet : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int ani;

public:
	CFireBullet(float x, float y, int FirePiranhaPlant_CurrentState);
	virtual void SetState(int state);
};

