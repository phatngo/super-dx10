#pragma once
#include "Brick.h"
#include "GameObject.h"
#include "Timer.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define FLASH_BRICK_STATE_FLASHING 606
#define COIN_STATE 607
#define FLASH_BRICK_STATE_NON_EXIST 700


#define PIECE_LEFT -1
#define PIECE_UP   -1
#define PIECE_DOWN  1
#define PIECE_RIGHT 1

#define COIN_ANI_SET_ID 6

#define FLASH_BRICK_ANI_GENERAL 0
#define DISPLAY_COIN_TIME 4000

class CFlashAnimationBrick : public CGameObject
{
	Timer transformToCoinTimer;
	void CreatePieces();
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	CFlashAnimationBrick();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
}; 
