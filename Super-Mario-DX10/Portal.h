#pragma once

#include "GameObject.h"
#include "Timer.h"

#define TOPLAYSCENE			0
#define TOEXTRASCENE		1
#define BACKTOPLAYSCENE		2

#define PORTAL_BBOX_WIDTH	26
#define PORTAL_BBOX_WIDTH_1_3 15
#define PORTAL_BBOX_HEIGHT	1

#define TO_EXTRA_SCENE 1
#define BACK_TO_MAIN_SCENE 2

class CPortal : public CGameObject
{
	int scene_id = -1;	// target scene to switch to 

	int width = 0;
	int height = 0;
public:
	float start_x, start_y;
	bool pipeUp = false;
	CPortal(int scene_id, float start_x = 0, float start_y = 0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
};