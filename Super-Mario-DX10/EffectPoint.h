#pragma once
#include "GameObject.h"

#define EFFECT_POINT_100  100
#define EFFECT_POINT_200  200
#define EFFECT_POINT_400  400
#define EFFECT_POINT_800  800
#define EFFECT_POINT_1000 1000

#define ANI_SET_ID_POINT_100  128
#define ANI_SET_ID_POINT_200  133
#define ANI_SET_ID_POINT_400  129
#define ANI_SET_ID_POINT_800  131
#define ANI_SET_ID_POINT_1000 132

#define POINT_GENERAL_ANI 0

#define POINT_SPEED_Y 0.05f
#define POINT_FLYING_DY 30

#define EFFECT_POINT_STATE_EXIST 1
#define EFFECT_POINT_STATE_NON_EXIST 0

class EffectPoint : public CGameObject
{
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	EffectPoint();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};

