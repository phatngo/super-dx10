#pragma once
#include "GameObject.h"

#define COURSE_CLEAR_ANI_SET_ID 73
#define COURSE_CLEAR_Y_COORDINATE 265
#define COURSE_CLEAR_DX_FROM_CARD 5
#define COURSE_CLEAR_ANI_GENERAL 0
class CourseClear : public CGameObject
{
public:
	void Render();
	CourseClear(float x, float y);
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};

