#pragma once
#include "GameObject.h"
#include "Camera.h"

#define FONT_ANI_SET_ID 134
#define FONT_WIDTH 8

#define DIGIT_0 0
#define DIGIT_1 1
#define DIGIT_2 2
#define DIGIT_3 3
#define DIGIT_4 4
#define DIGIT_5 5
#define DIGIT_6 6
#define DIGIT_7 7
#define DIGIT_8 8
#define DIGIT_9 9

#define ANI_0 0
#define ANI_1 1
#define ANI_2 2
#define ANI_3 3
#define ANI_4 4
#define ANI_5 5
#define ANI_6 6
#define ANI_7 7
#define ANI_8 8
#define ANI_9 0



class Font : public CGameObject
{
	int ani = 0;
public:
	Font(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt) {};
	virtual void GetBoundingBox(float& oLeft, float& oTop, float& oRight, float& oBottom) {};
	void SetAni(int ani);
};



