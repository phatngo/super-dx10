#pragma once
#include "GameObject.h"
#include "Timer.h"
#define PIECE_BBOX_WIDTH  8
#define PIECE_BBOX_HEIGHT 8

#define PIECE_ANI_FLYING  0

#define PIECE_STATE_FLYING    100
#define PIECE_STATE_NON_EXIST 200
#define PIECE_SPEED           0.075f
#define PIECE_ANI_SET_ID	  126
#define PIECE_TIME            500


class CPiece: public CGameObject
{
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	Timer appearanceTimer;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	CPiece(float ax, float ay);
	void SetState(int state);
};

