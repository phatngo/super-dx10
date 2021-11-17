#include "WorldScene.h"
#include <iostream>
#include <fstream>
#include "PlayScence.h"
#include "Textures.h"
#include "Sprites.h"
#include "Utils.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "FlashAnimationBrick.h"
#include "Map.h"
#include "Block.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Switch.h"
#include "Goomba.h"
#include "Koopas.h"
#include "PiranhaPlant.h"
#include "FirePiranhaPlant.h"
#include "Piece.h"
#include "HUD.h"
#include "EffectPoint.h"
#include "FireBullet.h"
#include "Card.h"
#include "Camera.h"
#include "CourseClear.h"
#include "Pipe.h"
#include "Textures.h"
using namespace std;

CWorldScene::CWorldScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	isSceneDone = false;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 1
#define SCENE_SECTION_TILEMAP_DATA 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_EXTRA_INFORMATION 7

#define WORLD_SCENE_OBJECT    10
#define WORLD_SCENE_PLAYER    0


#define EXTRA_INFO_FIRST_POINT_IN_HUD_POSITION 1
#define EXTRA_INFO_LAST_POINT_IN_HUD_POSITION 2
#define EXTRA_INFO_CAMERA_STANDARD_Y_COORDINATE 3
#define EXTRA_INFO_CAMERA_FURTHEST_Y_COORDINATE 4
#define EXTRA_INFO_MARIO_MAX_X_COORDINATE 5
#define HUD_INITIAL_POSITION_COORDINATE 6
#define EXTRA_INFO_LIVE_IN_HUD_POSITION 7
#define EXTRA_INFO_FIRST_TIMER_DIGIT_IN_HUD_POSITION 8
#define EXTRA_INFO_FIRST_POWER_ARROW_IN_HUD_POSITION 9
#define EXTRA_INFO_ICONP_IN_HUD_POSITION 10
#define EXTRA_INFO_ICON_MARIO_IN_HUD_POSITION 11

#define UNKNOWN_VALUE -1



#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CWorldScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str());
}

void CWorldScene::_ParseSection_TILEMAP_DATA(string line)
{

	int ID, rowMap, columnMap, columnTile, rowTile, totalTiles;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;
	f.open(path);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles;
	DebugOut(L"[INFO] _ParseSection_TILEMAP %d, %d \n", ID, rowMap);
	//Init Map Matrix
	int** TileMapData = new int* [rowMap];
	for (int i = 0; i < rowMap; i++)
	{
		TileMapData[i] = new int[columnMap];
		int j;
		for (j = 0; j < columnMap; j++) {
			f >> TileMapData[i][j];
			DebugOut(L"[INFO] _ParseSection_TILEMAP %d \n", TileMapData[i][j]);
		}
	}
	f.close();

	current_map = new CMap(ID, rowMap, columnMap, rowTile, columnTile, totalTiles);
	current_map->ExtractTileFromTileSet();
	current_map->SetTileMapData(TileMapData);
	mapWidth = current_map->GetMapWidth();
	DebugOut(L"[INFO] _ParseSection_TILEMAP_DATA done:: \n");

}

void CWorldScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CWorldScene::_ParseSection_ANIMATIONS(string line)
{
	CCamera::GetInstance()->GetCameraX();
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CWorldScene::_ParseSection_ANIMATION_SETS(string line)
{

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CWorldScene::_ParseSection_EXTRA_INFORMATION(string line)
{

	
}


void CWorldScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int object_type = atoi(tokens[0].c_str());
	CGameObject* obj = NULL;

	switch (object_type)
	{
	case WORLD_SCENE_OBJECT:
		break;
	case WORLD_SCENE_PLAYER:
		break;
	}
}

void CWorldScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[TILEMAP DATA]") { section = SCENE_SECTION_TILEMAP_DATA; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[EXTRA INFORMATION]") {
			section = SCENE_SECTION_EXTRA_INFORMATION; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_TILEMAP_DATA: _ParseSection_TILEMAP_DATA(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_EXTRA_INFORMATION: _ParseSection_EXTRA_INFORMATION(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CWorldScene::Update(DWORD dt)
{

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt);
	}

}

void CWorldScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	hud->Render();
}

/*
	Unload current scene
*/
void CWorldScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();


	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


vector<LPGAMEOBJECT> CWorldScene::GetSceneObjects()
{
	return this->objects;
}

void CWorldScene::SetSceneObjects(vector<LPGAMEOBJECT> newObjects)
{
	this->objects = newObjects;
}

void CWorldScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	switch (KeyCode)
	{
	case DIK_RIGHT:
		break;
	case DIK_LEFT:
		break;
	case DIK_UP:
		break;
	case DIK_DOWN:
		break;
	}
}




