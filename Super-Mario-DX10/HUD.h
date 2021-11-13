#pragma once
#include"GameObject.h"
#include"Sprites.h"
#include "Font.h"
#include "Timer.h"

#define FONT_BBOX_WIDTH						8
#define DEFAULT_TIME						300

#define SPRITE_ICONMARIO_ID					50003
#define SPRITE_ICONLUIGI_ID					50004
#define SPRITE_FILLARROW_ID					50005
#define ANI_P_ID							5006
#define NUMBER_OF_POWER_ARROW               6
#define PLAYSCENE_HUD	                    0
#define WORLDSCENE_HUD	                    1
#define CARD_ANI_SET_ID	                    58

#define ARROW_WIDTH                         8

#define HUD_HEIGHT                          32
#define HUD_CY                              395
#define POINT_DIGIT_NUMBER                  7
#define TIMER_DIGIT_NUMBER                  3

#define UNKNOWN_VALUE -1.0f
#define TOTAL_GAME_TIME 300
#define TIME_TO_CHANGE_SECOND 800
#define POWERMELTER_FILLED_ARROW_SPRITE 50005
#define POWERMELTER_FILLED_ICONP_SPRITE 50006
#define ICON_P_ON_TIME 100
#define ICON_P_OFF_TIME 500

class HUD : public CGameObject
{
	int type;
	vector<Font*>pointDigits;
	vector<Font*>moneyDigits;
	vector<Font*>gameTimerDigits;
	int power;
	float firstPointPositionX = UNKNOWN_VALUE;
	float firstPointPositionY = UNKNOWN_VALUE;
	float lastMoneyPositionX = UNKNOWN_VALUE;
	float lastMoneyPositionY = UNKNOWN_VALUE;
	float preScene_y;
	Font* mario_lives;
	float livePositionX;
	float livePositionY;
	float firstTimerPositionX;
	float firstTimerPositionY;
	float firstPowerArrowPositionX;
	float firstPowerArrowPositionY;
	float iconPPositionX;
	float iconPPositionY;
	int game_time;
	Timer timer;
	Timer iconPTimerOn;
	bool isIconPBlinking;
	Timer iconPTimerOff;
public:

	void SetYPosition(float y) {this->start_Y = y; }
	void SetPreSceneYPosition(float y) { this->preScene_y = y; }
	float GetPreSceneYPosition() { return this->preScene_y; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& oLeft, float& oTop, float& oRight, float& oBottom) {};
	void SetFirstPointPosition(float x, float y) { firstPointPositionX = x; firstPointPositionY = y; }
	void SetLivePosition(float x, float y) { livePositionX = x; livePositionY = y; }
	void SetLastMoneyPosition(float x, float y) { lastMoneyPositionX = x; lastMoneyPositionY = y; }
	void SetFirstTimerDigitPosition(float x, float y) { firstTimerPositionX = x; firstTimerPositionY = y; }
	void SetFirstPowerArrowPosition(float x, float y) { firstPowerArrowPositionX = x; firstPowerArrowPositionY = y; }
	void SetIconPPosition(float x, float y) { iconPPositionX = x; iconPPositionY = y; }
	HUD();
};

