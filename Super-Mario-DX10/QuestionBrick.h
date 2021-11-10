#pragma once
#include "GameObject.h"
#include "Timer.h"

#define QUESTION_BRICK_STATE_IDLE 602
#define QUESTION_BRICK_STATE_JUMPING 603
#define QUESTION_BRICK_STATE_FALLING 604
#define QUESTION_BRICK_STATE_STOP 6021
#define QUESTION_BRICK_JUMP_SPEED 0.06f

#define QUESTION_BRICK_ANI_SPINNING 0
#define QUESTION_BRICK_ANI_STOP 1
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define COIN_TAG 0
#define MUSHROOM_TAG 1
#define LEAF_TAG 4
#define SWITCH_TAG 6
#define COIN_ANI_SET 6
#define MUSHROOM_ANI_SET 37
#define LEAF_ANI_SET 36
#define SWITCH_ANI_SET 77

#define TIME_UNTIL_POINT_APPEAR 700

#define POINT_SPRITE_ID_100  50049


class CQuestionBrick : public CGameObject
{
	bool isObjectCreated;
	Timer pointAppearanceTimer;
	int repeatTime;
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	CQuestionBrick(int tag, int repeatTime);
	void SetState(int state);
	void CreateObject();
	void CreateCoin();
	void CreateMushroom();
	void CreateLeaf();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

