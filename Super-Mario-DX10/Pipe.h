#pragma once
#include "GameObject.h"

#define PIPE_BBOX_WIDTH  16
#define PIPE_BBOX_HEIGHT 16
#define PIPE_GENERAL_ANI 0


class CPipe : public CGameObject
{
	bool isAbleToGoIn = false;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	CPipe(int isAbleToGoIn);
};