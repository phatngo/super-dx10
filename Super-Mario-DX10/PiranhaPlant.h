#include "GameObject.h"

#define PIRANHA_STATE_UP 101
#define PIRANHA_STATE_DOWN 301


#define PIRANHA_ANI_NORMAL 0

#define PIRANHA_BBOX_WIDTH  16
#define PIRANHA_BBOX_HEIGHT 24
#define PIRANHA_HIGHEST_DY 31
#define PIRANHA_UP_SPEED 0.01f
#define EXTRA_PIRANHA_DY_MOVING_UP         4.0f
class CPiranhaPlant : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CPiranhaPlant();
	virtual void SetState(int state);
};