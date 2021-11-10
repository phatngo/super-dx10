#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "EffectPoint.h"
#include "Utils.h"

#define MARIO_WALKING_SPEED_START	0.0001f 
#define MARIO_WALKING_SPEED_MAX		0.15f
#define MARIO_RUNNING_SPEED_MAX		0.20f
#define MARIO_SPEED_MAX				0.25f
#define MARIO_ACCELERATION			0.0007f
#define MARIO_WALKING_SPEED_MIN		0.05f
#define MARIO_SPEED_RUN_FLY_MAX	    0.40f
#define MARIO_SPEED_RUN_FLY_MAX	    0.40f
#define MARIO_PIPE_SPEED	        0.02f


#define MARIO_JUMP_SPEED_Y		    0.43f 
#define MARIO_JUMP_DEFLECT_SPEED    0.3f

#define MARIO_GRAVITY			    0.002f
#define MARIO_ACCEL_WALK_X	        0.0005f

#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_TRANSFORM_DY_FROM_SMALL_TO_BIG 6.5f
#define MARIO_SIT_DOWN_DY 1.0f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_TRANSFORMING    500
#define MARIO_STATE_RELEASE_JUMP    600
#define MARIO_STATE_SIT             700

#define MARIO_ANI_BIG_IDLE_RIGHT		    14
#define MARIO_ANI_BIG_IDLE_LEFT			    22
#define MARIO_ANI_BIG_WALKING_RIGHT		    15
#define MARIO_ANI_BIG_WALKING_FAST_RIGHT    16
#define MARIO_ANI_BIG_WALKING_LEFT		    23
#define MARIO_ANI_BIG_WALKING_FAST_LEFT		24
#define MARIO_ANI_BIG_KICK_RIGHT		    74
#define MARIO_ANI_BIG_KICK_LEFT				78
#define MARIO_ANI_BIG_BRAKING_RIGHT			20
#define MARIO_ANI_BIG_BRAKING_LEFT			28
#define MARIO_ANI_BIG_JUMPINGUP_RIGHT		18
#define MARIO_ANI_BIG_JUMPINGDOWN_RIGHT		19
#define MARIO_ANI_BIG_JUMPINGUP_LEFT		26
#define MARIO_ANI_BIG_JUMPINGDOWN_LEFT		27
#define MARIO_ANI_BIG_SIT_RIGHT 	        21
#define MARIO_ANI_BIG_SIT_LEFT 	            29
#define MARIO_ANI_BIG_HOLD_IDLE_RIGHT		71
#define MARIO_ANI_BIG_HOLD_WALK_RIGHT	    72
#define MARIO_ANI_BIG_HOLD_JUMP_RIGHT		73
#define MARIO_ANI_BIG_HOLD_IDLE_LEFT		75
#define MARIO_ANI_BIG_HOLD_WALK_LEFT	    76
#define MARIO_ANI_BIG_HOLD_JUMP_LEFT		77
#define MARIO_ANI_BIG_MAX_SPEED_RIGHT		17
#define MARIO_ANI_BIG_MAX_SPEED_LEFT		25
#define MARIO_ANI_BIG_FLY_RIGHT		       105
#define MARIO_ANI_BIG_FLY_LEFT		       106

#define MARIO_ANI_SMALL_IDLE_RIGHT		    0
#define MARIO_ANI_SMALL_IDLE_LEFT	        7
#define MARIO_ANI_SMALL_WALKING_RIGHT	    1
#define MARIO_ANI_SMALL_WALKING_FAST_RIGHT	2
#define MARIO_ANI_SMALL_WALKING_LEFT	    8
#define MARIO_ANI_SMALL_WALKING_FAST_LEFT	9
#define MARIO_ANI_SMALL_KICK_RIGHT		    66
#define MARIO_ANI_SMALL_KICK_LEFT		    70
#define MARIO_ANI_SMALL_BRAKING_RIGHT		6
#define MARIO_ANI_SMALL_BRAKING_LEFT		13
#define MARIO_ANI_SMALL_JUMPINGUP_RIGHT		4
#define MARIO_ANI_SMALL_JUMPINGDOWN_RIGHT	5
#define MARIO_ANI_SMALL_JUMPINGUP_LEFT		11
#define MARIO_ANI_SMALL_JUMPINGDOWN_LEFT	12
#define MARIO_ANI_SMALL_HOLD_IDLE_RIGHT		63
#define MARIO_ANI_SMALL_HOLD_WALK_RIGHT	    64
#define MARIO_ANI_SMALL_HOLD_JUMP_RIGHT		65
#define MARIO_ANI_SMALL_HOLD_IDLE_LEFT		67
#define MARIO_ANI_SMALL_HOLD_WALK_LEFT	    68
#define MARIO_ANI_SMALL_HOLD_JUMP_LEFT		69
#define MARIO_ANI_SMALL_MAX_SPEED_RIGHT		3
#define MARIO_ANI_SMALL_MAX_SPEED_LEFT		10
#define MARIO_ANI_SMALL_FLY_RIGHT		    103
#define MARIO_ANI_SMALL_FLY_LEFT		    104

#define MARIO_ANI_TAIL_IDLE_RIGHT		    30
#define MARIO_ANI_TAIL_IDLE_LEFT		    38
#define MARIO_ANI_TAIL_WALKING_RIGHT	    31
#define MARIO_ANI_TAIL_WALKING_FAST_RIGHT	32
#define MARIO_ANI_TAIL_WALKING_LEFT	        39
#define MARIO_ANI_TAIL_WALKING_FAST_LEFT	40
#define MARIO_ANI_TAIL_KICK_RIGHT			82
#define MARIO_ANI_TAIL_KICK_LEFT		    86
#define MARIO_ANI_TAIL_BRAKING_RIGHT		36
#define MARIO_ANI_TAIL_BRAKING_LEFT			44
#define MARIO_ANI_TAIL_JUMPINGUP_RIGHT		34
#define MARIO_ANI_TAIL_JUMPINGDOWN_RIGHT	35
#define MARIO_ANI_TAIL_JUMPINGUP_LEFT		42
#define MARIO_ANI_TAIL_JUMPINGDOWN_LEFT		43
#define MARIO_ANI_TAIL_SIT_RIGHT 	        45
#define MARIO_ANI_TAIL_SIT_LEFT 	        37
#define MARIO_ANI_TAIL_HOLD_IDLE_RIGHT		79
#define MARIO_ANI_TAIL_HOLD_WALK_RIGHT	    80
#define MARIO_ANI_TAIL_HOLD_JUMP_RIGHT		81
#define MARIO_ANI_TAIL_HOLD_IDLE_LEFT		83
#define MARIO_ANI_TAIL_HOLD_WALK_LEFT	    84
#define MARIO_ANI_TAIL_HOLD_JUMP_LEFT		85
#define MARIO_ANI_TAIL_MAX_SPEED_RIGHT		33
#define MARIO_ANI_TAIL_MAX_SPEED_LEFT		41
#define MARIO_ANI_TAIL_FLY_UP_RIGHT		    107
#define MARIO_ANI_TAIL_FLY_DOWN_RIGHT       108
#define MARIO_ANI_TAIL_FLY_FLAPPING_RIGHT   109
#define MARIO_ANI_TAIL_FLY_UP_LEFT		    110
#define MARIO_ANI_TAIL_FLY_DOWN_LEFT        111
#define MARIO_ANI_TAIL_FLY_FLAPPING_LEFT    112


#define MARIO_ANI_TRANSFORM_BIG_RIGHT	    115
#define MARIO_ANI_TRANSFORM_BIG_LEFT	    116
#define MARIO_ANI_TRANSFORM_TAIL 	        117

#define MARIO_SPRITE_PIPING_SMALL           10900
#define MARIO_SPRITE_PIPING_BIG             11900
#define MARIO_SPRITE_PIPING_TAIL            12900




#define MARIO_ANI_DIE		8

#define MARIO_LEVEL_TRANSFORM_BIG 0
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_TAIL	3
#define MARIO_LEVEL_TRANSFORM_TAIL 5
#define MARIO_LEVEL_TRANSFORM_SMALL 6

#define MARIO_TAIL_BBOX_WIDTH  21
#define MARIO_TAIL_BBOX_HEIGHT 28

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 27 

#define MARIO_BIG_BBOX_SIT_HEIGHT 18 

#define MARIO_TAIL_BBOX_SIT_HEIGHT 18 
#define MARIO_TAIL_BBOX_SIT_WIDTH  22

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 16

#define MARIO_UNTOUCHABLE_TIME      5000
#define MARIO_TRANSFORMING_TIME		400

#define ALPHA             1.0f
#define UNTOUCHABLE_ALPHA 0.5f
#define KICK_TIME 300
#define PIPE_TIME 1500
#define MARIO_MAX_COORDINATE_X		2802

#define IS_PIPE_UP -1
#define IS_PIPE_DOWN 1
#define IS_NOT_PIPE 0

#define MARIO_DY_GET_OUT_FROM_PIPE 40



class CMario : public CGameObject
{
	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	ULONGLONG marioDT;
	float maxVx;
	int ani;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	Timer transformTimer;
	int totalPoint=0;
	int totalMoney=0;
	bool isKickingKoopas;
	bool isOnGround;
	Timer kickTimer;
	Timer pipeTimer;
	bool isChangeDirection;
	float postion_y;
	bool isSitDown;
	bool isStandUpAgain;
	float maxXcoordinate;
	bool isHold;
	bool isThrow;
	bool isReadyToHold = false;
	bool isReadyToRunMax = false;
	bool isFlyingToTheSky = false;
	bool isPipedUp = false;
	bool isPipedDown = false;
	int pipeDirection = -1;
	Timer stopPipingTimer;
public: 
	Timer GetStopPipingTimer() { return this->stopPipingTimer; }
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL, vector<LPGAMEOBJECT>* objects=NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	int GetLevel() { return level; }
	void Reset();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void AddPoint(float x, float y, int point = EFFECT_POINT_100);
	void AddMoney() { this->totalMoney++; }
	int GetPoint() { return this->totalPoint; }
	int GetMoney() { return this->totalMoney; }
	void SetMaxXCoordinate(float maxXCoordinate) { this->maxXcoordinate = maxXCoordinate; }
	void SetIsHold(bool isHold) { this->isHold = isHold; }
	void SetIsThrow(bool isThrow) { this->isThrow = isThrow; }
	void SetIsReadyToHold(bool isReadyToHold) { this->isReadyToHold = isReadyToHold; }
	bool IsThrow() { return isThrow; }
	bool IsHold() { return this->isHold; }
	bool IsReadyToHold() { return this->isReadyToHold; }
	void SetIsReadyToRunMax(bool isReadyToRunMax) { this->isReadyToRunMax = isReadyToRunMax; }
	bool IsReadyToRunMax() { return this->isReadyToRunMax; }
	bool IsPipeUp() { return this->isPipedUp;}
	bool IsPipeDown() { return this->isPipedDown; }
	void SetIsPipe(int pipeDirection);
	Timer GetPipeTimer() { return this->pipeTimer; }
};