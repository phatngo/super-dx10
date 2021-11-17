#pragma once
#include "Scence.h"
#include "GameObject.h"
#include "Map.h"
#include "Camera.h"
#include "HUD.h"
class CWorldScene:public CScene
{
protected:			
	vector<LPGAMEOBJECT> objects;
	CMap* current_map = NULL;
	HUD* hud = nullptr;
	CCamera* cam = NULL;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_TILEMAP_DATA(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_EXTRA_INFORMATION(string line);
public:
	CWorldScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual vector<LPGAMEOBJECT> GetSceneObjects();


	//Added by myself
	virtual void  SetSceneObjects(vector<LPGAMEOBJECT> objects);
	void AddObjects(CGameObject* gameObj) {
		this->objects.insert(this->objects.end() - 2, gameObj);
	}
	void SetHUD(HUD* hud) { this->hud = hud; }
	HUD* GetHUD() { return this->hud; }

};

class CWorldScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CWorldScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

