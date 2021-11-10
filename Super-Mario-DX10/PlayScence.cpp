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

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
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

#define OBJECT_TYPE_GRID    999
#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_QUESTION_BRICK	142
#define OBJECT_TYPE_BRICK_WITH_FLASH_ANIMATION	112
#define OBJECT_TYPE_BLOCK 4
#define OBJECT_TYPE_PIRANHA_PLANT 7
#define OBJECT_TYPE_FIRE_PIRANHA_PLANT 70
#define OBJECT_TYPE_COIN 6
#define OBJECT_TYPE_HUD 58
#define OBJECT_TYPE_CARD 57
#define OBJECT_TYPE_PORTAL 50


#define EXTRA_INFO_FIRST_POINT_IN_HUD_POSITION 1
#define EXTRA_INFO_LAST_POINT_IN_HUD_POSITION 2
#define EXTRA_INFO_CAMERA_STANDARD_Y_COORDINATE 3
#define EXTRA_INFO_CAMERA_FURTHEST_Y_COORDINATE 4
#define EXTRA_INFO_MARIO_MAX_X_COORDINATE 5
#define HUD_INITIAL_POSITION_COORDINATE 6

#define UNKNOWN_VALUE -1



#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_TILEMAP_DATA(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	CCamera::GetInstance()->GetCameraX();
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_EXTRA_INFORMATION(string line)
{
	
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines


	switch (atoi(tokens[0].c_str()))
	{
	case EXTRA_INFO_FIRST_POINT_IN_HUD_POSITION:
		hud->SetFirstPointPosition(atof(tokens[1].c_str()), atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_LAST_POINT_IN_HUD_POSITION:
		hud->SetLastMoneyPosition(atof(tokens[1].c_str()), atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_CAMERA_STANDARD_Y_COORDINATE:
		CCamera::GetInstance()->SetStandardCameraPositionY(atoi(tokens[1].c_str()));
		break;
	case EXTRA_INFO_CAMERA_FURTHEST_Y_COORDINATE:
		CCamera::GetInstance()->SetCameraFurthestPositionY(atoi(tokens[1].c_str()));
		break;
	case EXTRA_INFO_MARIO_MAX_X_COORDINATE:
		player->SetMaxXCoordinate(atoi(tokens[1].c_str()));
		break;
	case HUD_INITIAL_POSITION_COORDINATE:
		hud->SetPosition(atof(tokens[1].c_str()), atof(tokens[2].c_str()));
		break;
	default:
		break;
	}
}


void CPlayScene::_ParseSection_OBJECTS(string line)
{
	cam = CCamera::GetInstance();
	wstring path = ToWSTR(line);
	ifstream f;
	DebugOut(L"[INFO] Start object resources from : %s \n", path.c_str());
	f.open(path.c_str());
	char str[MAX_SCENE_LINE];

	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;

		vector<string> tokens = split(line);

		if (tokens.size() <= 3) continue;

		int object_type = atoi(tokens[0].c_str());

		float x = UNKNOWN_VALUE, y = UNKNOWN_VALUE;
		int ani_set_id = UNKNOWN_VALUE;
		if (object_type != OBJECT_TYPE_GRID) {
			x = atof(tokens[1].c_str());
			y = atof(tokens[2].c_str());
			ani_set_id = atoi(tokens[3].c_str());
		}

		int tag = -1;
		int objectInsideTag = -1;
		int repeat_time = -1;

		if (tokens.size() > 4)
			tag = atoi(tokens[4].c_str());

		if (tokens.size() > 5)
			objectInsideTag = atoi(tokens[5].c_str());

		if (tokens.size() > 7)
			repeat_time = atoi(tokens[7].c_str());
		
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();

		CGameObject* obj = NULL;
		CGameObject* obj_of_questionBrick = NULL;
		LPANIMATION_SET ani_set_of_obj;

		switch(object_type) 
		{
		case OBJECT_TYPE_MARIO:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y);
			player = (CMario*)obj;
			cam->SetPlayerStartX(x);
			break;
		case OBJECT_TYPE_BRICK:{
			if (tag == IS_PIPE) {
				obj = new CBrick(tag,objectInsideTag);
			}
			else {
				obj = new CBrick();
			}
			}
			break;
		case OBJECT_TYPE_COIN:
			obj = new CCoin();
			break;
		case OBJECT_TYPE_BLOCK:
			obj = new CBlock();
			break;
		case OBJECT_TYPE_GOOMBA: 
		{
			if (tag != -1) {
				obj = new CGoomba(tag);
				break;
			}
		}
		case OBJECT_TYPE_KOOPAS: {
			if (tag != -1) {
				obj = new CKoopas(tag);
				break;
			}
		}
		case OBJECT_TYPE_QUESTION_BRICK:
		{
			if (objectInsideTag != -1) {
				obj = new CQuestionBrick(objectInsideTag, repeat_time);
				break;
			}
		}
		case OBJECT_TYPE_BRICK_WITH_FLASH_ANIMATION:
			obj = new CFlashAnimationBrick();
		    break;
		case OBJECT_TYPE_PIRANHA_PLANT:
			obj = new CPiranhaPlant();
			break;
		case OBJECT_TYPE_FIRE_PIRANHA_PLANT: {
			if (tag != -1) {
				obj = new CFirePiranhaPlant(tag);
				break;
			}
		}
		case OBJECT_TYPE_CARD: {
			obj = new Card();
			break; 
		}
		case OBJECT_TYPE_HUD: {
			hud = new HUD();
			break;
		}
		case OBJECT_TYPE_GRID: {
			int gridCols = atoi(tokens[1].c_str());
			int gridRows = atoi(tokens[2].c_str());
			grid = new Grid(gridCols, gridRows);
			break;
		}
		case OBJECT_TYPE_PORTAL: {
			int scene_id = atoi(tokens[4].c_str());
			int isToExtraScene = atoi(tokens[5].c_str());
			float start_x = 0, start_y = 0;
			start_x = (float)atoi(tokens[6].c_str());
			start_y = (float)atoi(tokens[7].c_str());
			obj = new CPortal(scene_id, start_x, start_y);
			int pu = atoi(tokens[8].c_str());
			if (pu == 1)
				((CPortal*)obj)->pipeUp = true;
			else
				((CPortal*)obj)->pipeUp = false;
			obj->SetTag(isToExtraScene);
			break;
		}
		default:
			obj = NULL;
			break;
		}
		if (obj != NULL && object_type != OBJECT_TYPE_GRID && object_type != OBJECT_TYPE_HUD) {
			obj->SetPosition(x, y);
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
			objects.push_back(obj);
		}
		if (hud != NULL)
		{
			hud->SetPosition(x, y);
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			hud->SetAnimationSet(ani_set);
		}
		if (obj != NULL && object_type != OBJECT_TYPE_MARIO && object_type != OBJECT_TYPE_GRID && object_type != OBJECT_TYPE_HUD)
		{
			int gridCol = (int)atoi(tokens[tokens.size() - 1].c_str());
			int gridRow = (int)atoi(tokens[tokens.size() - 2].c_str());
			//add unit into grid
			Unit* unit = new Unit(grid, obj, gridRow, gridCol);
		}
	}
	f.close();
	grid->Out();
}

void CPlayScene::Load()
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
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[EXTRA INFORMATION]") {
			section = SCENE_SECTION_EXTRA_INFORMATION; continue;}
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

void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	units.clear();
	objectsRenderFirst.clear();
	objectsRenderSecond.clear();
	objectsRenderThird.clear();
	objects.clear();
	grid->Get(cam, units);

	
	for (UINT i = 0; i < units.size(); i++)
	{

		LPGAMEOBJECT obj = units[i]->GetObj();
		objects.push_back(obj);
		//CalRevivable

		for each (auto object in objects)
		{
			//Still having an error when calling summon koopas related to Grid, so that's why it is commented
			/*
			if (dynamic_cast<CKoopas*>(object) && !dynamic_cast<CKoopas*>(object)->CalRevivable()
				&& object->isDestroyed == false)
				object->IsDestroyed = true;
			*/
		}

		if (dynamic_cast<CGoomba*> (obj) || dynamic_cast<CKoopas*> (obj)
			|| dynamic_cast<CPiranhaPlant*> (obj)
			|| dynamic_cast<CFirePiranhaPlant*> (obj)
			|| dynamic_cast<CCoin*> (obj)
			|| dynamic_cast<CMushroom*> (obj) && obj->state == MUSHROOM_STATE_MOVING
			|| dynamic_cast<CLeaf*> (obj) && (obj->state == LEAF_STATE_FLY_UP)
			|| dynamic_cast<CSwitch*> (obj))
			objectsRenderFirst.push_back(obj);
		else if (dynamic_cast<CBrick*> (obj)
			|| (dynamic_cast<CQuestionBrick*> (obj) || (dynamic_cast<CFlashAnimationBrick*> (obj)))
			)
			objectsRenderSecond.push_back(obj);
		else if (dynamic_cast<CFireBullet*> (obj)
			|| dynamic_cast<CMushroom*>(obj)
			|| dynamic_cast<CLeaf*> (obj) && (obj->state == LEAF_STATE_FLY_DOWN_LEFT || obj->state == LEAF_STATE_FLY_DOWN_RIGHT)
			|| dynamic_cast<EffectPoint*>(obj) || dynamic_cast<CPiece*>(obj)
			|| dynamic_cast<Card*>(obj)
			|| dynamic_cast<CourseClear*>(obj)
			|| dynamic_cast<CPortal*>(obj))
			objectsRenderThird.push_back(obj);
	}

	
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	player->Update(dt, &coObjects, &objects);
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	float cx, cy;
	if (player == NULL) return;
	player->GetPosition(cx, cy);
	float playerStartX = player->GetStartX();
	cam->SetCameraPosition(cx, mapWidth);
	grid->UpdateGrid(units);
	hud->Update(dt);
}

void CPlayScene::Render()
{
	if (player == NULL) return;
	current_map->Render();
	player->Render();

	for (unsigned int i = 0; i < objectsRenderFirst.size(); i++)
		objectsRenderFirst[i]->Render();
	for (unsigned int i = 0; i < objectsRenderSecond.size(); i++)
		objectsRenderSecond[i]->Render();
	for (unsigned int i = 0; i < objectsRenderThird.size(); i++)
		objectsRenderThird[i]->Render();
	

	hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


vector<LPGAMEOBJECT> CPlayScene::GetSceneObjects()
{
	return this->objects;
}

void CPlayScene::SetSceneObjects(vector<LPGAMEOBJECT> newObjects)
{
	this->objects = newObjects;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	float speedX, speedY;
	mario->GetSpeed(speedX, speedY);
	switch (KeyCode)
	{
	case DIK_S:
		//if(mario->GetState()!=MARIO_STATE_RELEASE_JUMP)
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A: 
		mario->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (KeyCode == DIK_Q) {
		if (mario->IsHold()) {
			mario->SetIsHold(false);
			mario->SetIsThrow(true);
			mario->SetIsReadyToHold(false);
		}
	}
	if (KeyCode == DIK_RIGHT && mario->IsHold()) {
		mario->SetState(MARIO_STATE_IDLE);
	}
	if (KeyCode == DIK_LEFT && mario->IsHold()) {
		mario->SetState(MARIO_STATE_IDLE);
	}
	if (KeyCode == DIK_Z) {
		mario->SetIsReadyToRunMax(false);
	}
}



void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT) && !CGame::GetInstance()->GetCurrentScene()->GetSceneDone() && !mario->IsPipeUp() && !mario->IsPipeDown()) {
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT) && !CGame::GetInstance()->GetCurrentScene()->GetSceneDone() && !mario->IsPipeUp() && !mario->IsPipeDown())
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN)) 
		mario->SetState(MARIO_STATE_SIT);
	//If want to change the following key, please also change on OnKeyUp function and the Update function of mario
	else if (game->IsKeyDown(DIK_Q) && mario->IsReadyToHold()) {
			mario->SetIsHold(true);
			mario->SetIsThrow(false);
	}
	else if (game->IsKeyDown(DIK_T)) {
		//This is for testing when teleporting to another position
		mario->SetPosition(2258.0f, 50.0f);
		//mario->SetPosition(1488, 150);
	}
	else if (game->IsKeyDown(DIK_Z)) {
		if (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT)) {
			mario->SetIsReadyToRunMax(true);
		}
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}

