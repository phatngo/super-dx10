#pragma once
#include"GameObject.h"
#include"Sprites.h"
#include "Font.h"

#define FONT_BBOX_WIDTH						8
#define DEFAULT_TIME						300

#define SPRITE_ICONMARIO_ID					50003
#define SPRITE_ICONLUIGI_ID					50004
#define SPRITE_FILLARROW_ID					50005
#define ANI_P_ID							5006

#define PLAYSCENE_HUD	                    0
#define WORLDSCENE_HUD	                    1

#define CARD_ANI_SET_ID	                    58

#define HUD_HEIGHT                          32
#define HUD_CY                              395
#define POINT_DIGIT_NUMBER                  7

#define UNKNOWN_VALUE -1.0f



class HUD : public CGameObject
{
	int type;
	vector<Font*>pointDigits;
	vector<Font*>moneyDigits;
	float firstPointPositionX = UNKNOWN_VALUE;
	float firstPointPositionY = UNKNOWN_VALUE;
	float lastMoneyPositionX = UNKNOWN_VALUE;
	float lastMoneyPositionY = UNKNOWN_VALUE;
	float preScene_y;
public:
	void SetYPosition(float y) {this->start_Y = y; }
	void SetPreSceneYPosition(float y) { this->preScene_y = y; }
	float GetPreSceneYPosition() { return this->preScene_y; }

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& oLeft, float& oTop, float& oRight, float& oBottom) {};
	void SetFirstPointPosition(float x, float y) { firstPointPositionX = x; firstPointPositionY = y; }
	void SetLastMoneyPosition(float x, float y) { lastMoneyPositionX = x; lastMoneyPositionY = y; }
	HUD();
};

