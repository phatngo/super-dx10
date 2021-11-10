#pragma once
#include "GameObject.h"


#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define BRICK_GENERAL_ANI 0


class CBrick : public CGameObject
{
	int tag = -1;
	bool isAbleToGoIn = false;
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	CBrick(int tag, int isAbleToGoIn);
	CBrick(){}
	bool IsPipe() { return this->tag != -1; }
	bool IsAbleToGoIn() { return this->isAbleToGoIn; }
};