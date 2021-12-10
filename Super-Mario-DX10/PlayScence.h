#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "Camera.h"
#include "Grid.h"
#include "HUD.h"

#define IS_PIPE 10

class CPlayScene: public CScene
{
protected: 
	CMario *player = NULL;					// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objectsRenderFirst;
	vector<LPGAMEOBJECT> objectsRenderSecond;
	vector<LPGAMEOBJECT> objectsRenderThird;

	//Grid
	vector<Unit*> units;
	Unit* unit = NULL;
	Grid* grid = NULL;

	HUD* hud = nullptr;
	CCamera* cam = NULL;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_TILEMAP_DATA(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_EXTRA_INFORMATION(string line);
private:
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	

	//Added by myself
	Unit* GetUnit() { return unit; }
	Grid* GetGrid() { return grid; }

	//Added by myself
	
	void AddObjects(CGameObject* gameObj) { 
		this->objects.insert(this->objects.end()-2, gameObj);
	}
	void SetPlayer(CMario* m) { player = m; }
	void SetHUD(HUD* hud) { this->hud = hud; }
	HUD* GetHUD() { return this->hud; }

	virtual CMario * GetPlayer() { return player; } 
	

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

