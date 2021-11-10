#pragma once

#include <d3dx10.h>
#include "KeyEventHandler.h"
#include <vector>
#include "GameObject.h"
#include "Mario.h"

#define UNKNOW_VALUE -1 


class CScene
{
protected:
	CKeyEventHandler * key_handler = NULL;
	int id;
	LPCWSTR sceneFilePath;
	bool isCameraAutoMove = false;
	bool isSceneDone = false;
	int mapWidth = UNKNOW_VALUE;
public: 
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	void SetCamerAutoMove(bool l) { this->isCameraAutoMove = l; }
	virtual vector<LPGAMEOBJECT> GetSceneObjects();
	virtual void  SetSceneObjects(vector<LPGAMEOBJECT> objects)=0;
	virtual CMario* GetPlayer() = 0;
	virtual void SetSceneDone() { this->isSceneDone = true; }
	bool GetSceneDone() { return isSceneDone; }
	int GetMapWidth() { return mapWidth; }
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