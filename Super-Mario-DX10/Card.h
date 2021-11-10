#pragma once
#include "GameObject.h"
#define CARD_BBOX_WIDTH  16
#define CARD_BBOX_HEIGHT 16

#define CARD_ANI_GENERAL     0
#define CARD_ANI_MUSHROOM    4
#define CARD_ANI_PIRANHA     5
#define CARD_ANI_STAR        6

#define CARD_STATE_IDLE      100
#define CARD_STATE_FLY_UP    200   

#define CARD_FLY_UP_SPEED_Y  0.05f

class Card: public CGameObject
{
	int currentFrame;
  public:
	void Render();
	Card();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	void DisplayCourseClear();
};

