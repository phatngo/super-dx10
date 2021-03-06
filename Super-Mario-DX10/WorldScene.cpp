#include "WorldScene.h"
#include <iostream>
#include <fstream>
#include "Sprites.h"
#include "Utils.h"
#include "Portal.h"
#include "Map.h"
#include "HUD.h"
#include "Camera.h"
#include "Textures.h"
#include "WorldMapObject.h"
#include "WorldPlayer.h"
using namespace std;

CWorldScene::CWorldScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CWorldScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/


#define WORLD_SCENE_OBJECT    10
#define WORLD_SCENE_PLAYER    0
#define WORLD_SCENE_HUD 58


#define EXTRA_INFO_FIRST_POINT_IN_HUD_POSITION 1
#define EXTRA_INFO_LAST_POINT_IN_HUD_POSITION 2
#define HUD_INITIAL_POSITION_COORDINATE 6
#define EXTRA_INFO_LIVE_IN_HUD_POSITION 7
#define EXTRA_INFO_FIRST_TIMER_DIGIT_IN_HUD_POSITION 8
#define EXTRA_INFO_FIRST_POWER_ARROW_IN_HUD_POSITION 9
#define EXTRA_INFO_ICONP_IN_HUD_POSITION 10
#define EXTRA_INFO_ICON_MARIO_IN_HUD_POSITION 11

#define UNKNOWN_VALUE -1


#define MAX_SCENE_LINE 1024


void CWorldScene::_ParseSection_TEXTURES(string line)
{
	CScene::_ParseSection_TEXTURES(line);
}

void CWorldScene::_ParseSection_TILEMAP_DATA(string line)
{
	CScene::_ParseSection_TILEMAP_DATA(line);
}

void CWorldScene::_ParseSection_SPRITES(string line)
{
	CScene::_ParseSection_SPRITES(line);
}

void CWorldScene::_ParseSection_ANIMATIONS(string line)
{
	CScene::_ParseSection_ANIMATIONS(line);	
}

void CWorldScene::_ParseSection_ANIMATION_SETS(string line)
{
	CScene::_ParseSection_ANIMATION_SETS(line);
}

void CWorldScene::_ParseSection_EXTRA_INFORMATION(string line)
{
	DebugOut(L"[INFO] Start loading extra info \n");

	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines

	switch (atoi(tokens[0].c_str()))
	{
	case EXTRA_INFO_FIRST_POINT_IN_HUD_POSITION:
		hud->SetFirstPointPosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_LAST_POINT_IN_HUD_POSITION:
		hud->SetLastMoneyPosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_LIVE_IN_HUD_POSITION:
		hud->SetLivePosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	case HUD_INITIAL_POSITION_COORDINATE:
		hud->SetPosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_FIRST_TIMER_DIGIT_IN_HUD_POSITION:
		hud->SetFirstTimerDigitPosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_FIRST_POWER_ARROW_IN_HUD_POSITION:
		hud->SetFirstPowerArrowPosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_ICONP_IN_HUD_POSITION:
		hud->SetIconPPosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_ICON_MARIO_IN_HUD_POSITION:
		hud->SetIconMarioPosition((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
		break;
	default:
		break;
	}
}

void CWorldScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);
	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());
	//objects.clear();
	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int tag = 0, option_tag_1 = 0, option_tag_2 = 0;
	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = (int)atoi(tokens[3].c_str());
	if (tokens.size() >= 5)
		tag = (int)atof(tokens[4].c_str());
	if (tokens.size() >= 6)
		option_tag_1 = (int)atof(tokens[5].c_str());
	if (tokens.size() >= 7)
		option_tag_2 = (int)atof(tokens[6].c_str());


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = NULL;

	switch (object_type)
	{
	case WORLD_SCENE_PLAYER:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] PLAYER object was created before!\n");
			return;
		}
		float backup_x, backup_y;
		BackUp::GetInstance()->GetWorldPlayerPostion(backup_x, backup_y);
		if (backup_x == 0.0f && backup_y == 0.0f) {
			obj = new CWorldPlayer(x, y);
		}
		else {
			obj = new CWorldPlayer(backup_x, backup_y);
		}
		player = (CWorldPlayer*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case WORLD_SCENE_OBJECT:
		if (tag == OBJECT_TYPE_PORTAL || tag == OBJECT_TYPE_STOP)
		{
			bool cgLeft, cgRight, cgUp, cgDown;
			cgLeft = atof(tokens[5].c_str());
			cgUp = atof(tokens[6].c_str());
			cgRight = atof(tokens[7].c_str());
			cgDown = atof(tokens[8].c_str());
			int sceneid = (int)atof(tokens[9].c_str());
			obj = new CWorldMapObject(sceneid);
			((CWorldMapObject*)obj)->SetMove(cgLeft, cgUp, cgRight, cgDown);
			obj->SetTag(tag);
		}
		else
		{
			obj = new CWorldMapObject();
			obj->SetTag(tag);
			if (tag == OBJECT_TYPE_HAMMER)
				obj->SetSpeed(MARIO_WALKING_SPEED_MIN / 2, 0);
		}
		DebugOut(L"[INFO] World object created!\n");
		break;
	case WORLD_SCENE_HUD:
		hud = new HUD();
		DebugOut(L"[INFO] HUD created!\n");
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	// General object setup
	if (hud != NULL)
	{
		hud->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		hud->SetAnimationSet(ani_set);
	}
	if(object_type != WORLD_SCENE_HUD && object_type != WORLD_SCENE_PLAYER) {
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}
	if (object_type == WORLD_SCENE_PLAYER) {
		float backup_x, backup_y;
		BackUp::GetInstance()->GetWorldPlayerPostion(backup_x, backup_y);
		if (backup_x == 0.0f || backup_y == 0.0f) {
			obj->SetPosition(x, y);
		}
		else {
			obj->SetPosition(backup_x, backup_y);
		}
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}
}

void CWorldScene::Load()
{
	cam = CCamera::GetInstance();
	cam->SetCameraPosition(CAMERA_WORLD_SCENE_POSTION_X, CAMERA_WORLD_SCENE_POSTION_Y);
	CScene::Load();
}

void CWorldScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	player->Update(dt, &coObjects);
	hud->Update(dt);
}

void CWorldScene::Render()
{
	current_map->Render();
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	player->Render();
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

	delete current_map;
	delete hud;

	player = nullptr;
	current_map = nullptr;
	hud = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}



void CWorldScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CWorldPlayer* player = ((CWorldScene*)scence)->GetWorldPlayer();
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (player != NULL)
	{
		switch (KeyCode)
		{
		case DIK_RIGHT:
			if (player->cgRight)
				player->SetState(PLAYER_STATE_RIGHT);
			break;
		case DIK_LEFT:
			if (player->cgLeft)
				player->SetState(PLAYER_STATE_LEFT);
			break;
		case DIK_UP:
			if (player->cgUp)
				player->SetState(PLAYER_STATE_UP);
			break;
		case DIK_DOWN:
			if (player->cgDown)
				player->SetState(PLAYER_STATE_DOWN);
			break;
		case DIK_S:
			if (player->sceneId > 0 || BackUp::GetInstance()->GetCurrentSceneId() > 0)
				player->ChooseScene();
			DebugOut(L"Enter Map");
			break;

		}
	}
}




