#pragma once
#include "GameObject.h"
#include "Game.h"
#include "FireBullet.h"
#include "Timer.h"
#define TAG_FIRE_PIRANHA_RED 1
#define TAG_FIRE_PIRANHA_GREEN 0

#define FIRE_PIRANHA_ANI_UP_RIGHT 0
#define FIRE_PIRANHA_ANI_DOWN_RIGHT 1
#define FIRE_PIRANHA_ANI_UP_LEFT 2
#define FIRE_PIRANHA_ANI_DOWN_LEFT 3

#define FIRE_PIRANHA_STATE_UP 101
#define FIRE_PIRANHA_STATE_DOWN 301
#define FIRE_PIRANHA_STATE_SHOOT 300

#define FIRE_PIRANHA_BBOX_WIDTH       16
#define FIRE_PIRANHA_BBOX_HEIGHT      24
#define RED_FIRE_PIRANHA_HIGHEST_DY   31
#define GREEN_FIRE_PIRANHA_HIGHEST_DY 24
#define FIRE_PIRANHA_UP_SPEED         0.05f
#define EXTRA_DY_MOVING_UP         3.0f

#define UP_DELAY_TIME    5000
#define DOWN_DELAY_TIME  3000
#define SHOOT_DELAY_TIME 400


class CFirePiranhaPlant : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CreateFireBullet();
	int ani;
	Timer up_Timer;
	Timer down_Timer;
	Timer shoot_Timer;
	bool isFireBulletCreated;
	float firePiranhaPlant_y;
public:
	CFirePiranhaPlant(int objTag);
	virtual void SetState(int state);
};
