#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Timer.h"

#define GOOMBA_TAG_YELLOW                  0
#define GOOMBA_TAG_RED                     1

#define GOOMBA_WALKING_SPEED               0.04f

#define GOOMBA_FALLING_LOW_SPEED_X         0.01f
#define GOOMBA_FALLING_LOW_SPEED_Y         0.09f
#define GOOMBA_JUMPING_LOW_SPEED_X         0.01f
#define GOOMBA_JUMPING_LOW_SPEED_Y         0.09f
#define GOOMBA_FALLING_HIGH_SPEED_X        0.01f
#define GOOMBA_FALLING_HIGH_SPEED_Y        0.1f
#define GOOMBA_JUMPING_HIGH_SPEED_X        0.01f
#define GOOMBA_JUMPING_HIGH_SPEED_Y        0.3f

#define GOOMBA_FALLING_KILLED_BY_KOOPAS_SPEED_X         0.05f
#define GOOMBA_FALLING_KILLED_BY_KOOPAS_SPEED_Y         0.09f
#define GOOMBA_JUMPING_KILLED_BY_KOOPAS_SPEED_X         0.05f
#define GOOMBA_JUMPING_KILLED_BY_KOOPAS_SPEED_Y         0.09f

#define GOOMBA_GRAVITY                     0.001f
#define GOOMBA_DIE_DEFLECT_SPEED	       0.35f

#define GOOMBA_BBOX_NORMAL_WIDTH           16
#define GOOMBA_BBOX_NORMAL_HEIGHT          15
#define GOOMBA_BBOX_RED_UNWALK_WIDTH       18 
#define GOOMBA_BBOX_RED_UNWALK_HEIGHT      22
#define GOOMBA_BBOX_RED_WALK_WING_WIDTH    18
#define GOOMBA_BBOX_RED_WALK_WING_HEIGHT   17
#define GOOMBA_BBOX_HEIGHT_DIE             9

#define GOOMBA_STATE_YELLOW_WALKING        100
#define GOOMBA_STATE_DIE                   200
#define GOOMBA_STATE_NON_EXIST             300
#define GOOMBA_STATE_JUMPING_LOW           400
#define GOOMBA_STATE_FALLING_LOW           500
#define GOOMBA_STATE_JUMPING_HIGH          600
#define GOOMBA_STATE_FALLING_HIGH          700
#define GOOMBA_STATE_RED_WALKING           800
#define GOOMBA_STATE_RED_WALK_WING         900
#define GOOMBA_STATE_RED_DIE               1000
#define GOOMBA_STATE_JUMPING_KILLED_BY_KOOPAS     1100
#define GOOMBA_STATE_FALLING_KILLED_BY_KOOPAS     1200
#define GOOMBA_STATE_JUMPING_AWAY_FROM_QUESTION_BRICK     1300
#define GOOMBA_STATE_FALLING_AWAY_FROM_QUESTION_BRICK     1400


#define GOOMBA_ANI_YELLOW_WALKING          0
#define GOOMBA_ANI_YELLOW_DIE              1
#define GOOMBA_ANI_RED_JUMPING             2
#define GOOMBA_ANI_RED_WALK_WING           3
#define GOOMBA_ANI_RED_WALK                4
#define GOOMBA_ANI_RED_DIE                 5

#define GOOMBA_DELAY_TIME                  400
#define GOOMBA_WALK_WING_TIME              1000
#define MAX_LOW_FALLING_TIME               3


#define GOOMBA_MAX_JUMP_LOW_DY             15
#define GOOMBA_MAX_JUMP_HIGH_DY            35
#define GOOMBA_MAX_JUMP_KILLED_BY_KOOPAS_DY 25



class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int lowFallingTime;
	int killingKoopasDirection;
	bool isOnGround;
	Timer transformToNonExistTimer;
	Timer redWalkWingTimer;
	bool isQuestionBrickAboveTouched;
	bool isTouchedByKoopas;
	void updateYellowGoomba(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void updateRedGoomba(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	bool isTouchedByLeaf;
	bool isQuestionBrickBelowTouched;
	bool isTouchedAnotherGoomba;
	float touchedBrickTop;
public: 	
	CGoomba(int tag);
	virtual void SetState(int state);
	int GetTag() { return this->tag; }
	void SetKillingKoopasDiretion(int koopasNx) { this->killingKoopasDirection = koopasNx; }
	void SetIsTouchedByKoopas() { isTouchedByKoopas = true; }
	bool IsTouchedByKoopas() { return isTouchedByKoopas; }
};