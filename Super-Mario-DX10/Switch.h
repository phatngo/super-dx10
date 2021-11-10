#pragma once
#include "GameObject.h"


class CSwitch : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CSwitch();
	virtual void SetState(int state);
};