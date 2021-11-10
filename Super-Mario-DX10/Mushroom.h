#pragma once
#include "GameObject.h"

#define MUSHROOM_X_VELOCITY 0.05f
#define MUSHROOM_Y_VELOCITY 0.05f
#define MUSHROOM_GRAVITY 0.001f

#define MUSHROOM_ANI_SET_ID 37

#define MUSHROOM_ANI_NORMAL 0
#define MUSHROOM_ANI_ABNORMAL 1

#define MUSHROOM_STATE_IDLE 100
#define MUSHROOM_STATE_MOVING 200
#define MUSHROOM_STATE_NON_EXIST 301

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

class CMushroom : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CMushroom();
	virtual void SetState(int state);
};
