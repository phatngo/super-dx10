#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  16
#define COIN_BBOX_HEIGHT 5

#define COIN_JUMP_SPEED_Y	0.2f
#define COIN_JUMP_MAX_DY    70
#define COIN_JUMP_DISAPPEAR_DY 35

#define COIN_STATE_NON_EXIST 0
#define COIN_STATE_EXIST 1
#define COIN_STATE_JUMPING 2
#define COIN_STATE_FALLING 3

#define COIN_ANI_SET_ID 6

#define COIN_ANI_GENERAL 0

#define TAG_COIN_IN_BRICK 100



class CCoin : public CGameObject
{
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	bool isAppear;
public:
	CCoin();
	CCoin(int tag);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
}; 
