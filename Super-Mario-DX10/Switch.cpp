#include "Switch.h"
CSwitch::CSwitch()
{

}

void CSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Switch can interact with the world and to each of them too!
	// 


}

void CSwitch::Render()
{

}

void CSwitch::SetState(int state)
{
	CGameObject::SetState(state);

}
