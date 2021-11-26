#pragma once


#include "KeyEventHandler.h"
#include <vector>
#include "GameObject.h"
#include "Mario.h"
#include "Map.h"





#define UNKNOW_VALUE -1 

#define SCENCE_TYPE_PLAY 2
#define SCENCE_TYPE_WORLD 1
#define SCENCE_TYPE_INTRO 0

#define WORLD_SCENE_ID   0


class CScene
{
protected:
	CKeyEventHandler * key_handler = NULL;
	int id;
	LPCWSTR sceneFilePath;
	bool isCameraAutoMove = false;
	bool isSceneDone = false;
	int mapWidth = UNKNOW_VALUE;
	CMap* current_map = NULL;
	vector<LPGAMEOBJECT> objects;
public: 
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	void SetCamerAutoMove(bool l) { this->isCameraAutoMove = l; }

	virtual CMario* GetPlayer() = 0;
	virtual void SetSceneDone(bool isSceneDone) { this->isSceneDone = isSceneDone; }
	bool GetSceneDone() { return isSceneDone; }
	int GetMapWidth() { return mapWidth; }
	
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_TILEMAP_DATA(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);


	//Each scene has its own objects, extra information and mechanism 
	//=> the functions below must be defined specifically for each scene class 
	virtual void _ParseSection_OBJECTS(string line)=0;
	virtual void _ParseSection_EXTRA_INFORMATION(string line)=0;
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	
};
typedef CScene * LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene *s) :CKeyEventHandler() { scence = s; }
};