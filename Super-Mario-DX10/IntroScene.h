#pragma once
#include "Scence.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Mario.h"
#include "Game.h"
#include "Timer.h"
#include "Camera.h"



#define OBJECT_TYPE_GROUND	 4

#define MAX_SCENE_LINE       1024
#define ANISET_BACKGROUND_ID 50
#define ANISET_GROUND_ID	 21
#define ANISET_ARROW_ID	     51

#define THREE_X	133
#define THREE_Y	110
#define ARROW_Y 143
#define ARROW_X	65
#define BACKGROUND_X 128.0f
#define BACKGROUND_Y 88.0f

#define GROUND_WIDTH	256

#define SCROLLING_TIME	2000
#define SWITCH_TIME	300

#define BACKGROUND_GENERAL_ANI 3
#define ARROW_FLASHING_ANI 0
#define ARROW_STATIC_ANI   1 



class CIntroScene : public CScene {
public:

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_EXTRA_INFORMATION(string line) {};
public:
	CCamera* cam = NULL;
	LPANIMATION_SET BackGround;
	LPANIMATION Three;
	LPANIMATION_SET Arrow;
	Timer switchTimer;

	CIntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	CMario* GetPlayer() { return NULL; }
};

class CIntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CIntroSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};