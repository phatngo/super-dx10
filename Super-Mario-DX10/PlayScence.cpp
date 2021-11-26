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
#define EXTRA_INFO_LIVE_IN_HUD_POSITION 7
#define EXTRA_INFO_FIRST_TIMER_DIGIT_IN_HUD_POSITION 8
#define EXTRA_INFO_FIRST_POWER_ARROW_IN_HUD_POSITION 9
#define EXTRA_INFO_ICONP_IN_HUD_POSITION 10
#define EXTRA_INFO_ICON_MARIO_IN_HUD_POSITION 11

#define UNKNOWN_VALUE -1



#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	CScene::_ParseSection_TEXTURES(line);
}

void CPlayScene::_ParseSection_TILEMAP_DATA(string line)
{

	CScene::_ParseSection_TILEMAP_DATA(line);

}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	CScene::_ParseSection_SPRITES(line);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	CScene::_ParseSection_ANIMATIONS(line);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{

	CScene::_ParseSection_ANIMATION_SETS(line);
}

void CPlayScene::_ParseSection_EXTRA_INFORMATION(string line)
{
	
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines


	switch (atoi(tokens[0].c_str()))
	{
	case EXTRA_INFO_FIRST_POINT_IN_HUD_POSITION:
		hud->SetFirstPointPosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_LAST_POINT_IN_HUD_POSITION:
		hud->SetLastMoneyPosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_CAMERA_STANDARD_Y_COORDINATE:
		CCamera::GetInstance()->SetStandardCameraPositionY(atoi(tokens[1].c_str()));
		break;
	case EXTRA_INFO_CAMERA_FURTHEST_Y_COORDINATE:
		CCamera::GetInstance()->SetCameraFurthestPositionY(atoi(tokens[1].c_str()));
		break;
	case EXTRA_INFO_MARIO_MAX_X_COORDINATE:
		player->SetMaxXCoordinate((float)atof(tokens[1].c_str()));
		break;
	case EXTRA_INFO_LIVE_IN_HUD_POSITION:
		hud->SetLivePosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
		break;
	case HUD_INITIAL_POSITION_COORDINATE:
		hud->SetPosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_FIRST_TIMER_DIGIT_IN_HUD_POSITION:
		hud->SetFirstTimerDigitPosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_FIRST_POWER_ARROW_IN_HUD_POSITION:
		hud->SetFirstPowerArrowPosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_ICONP_IN_HUD_POSITION:
		hud->SetIconPPosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
		break;
	case EXTRA_INFO_ICON_MARIO_IN_HUD_POSITION:
		hud->SetIconMarioPosition((float) atof(tokens[1].c_str()), (float) atof(tokens[2].c_str()));
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
			x = (float) atof(tokens[1].c_str());
			y = (float) atof(tokens[2].c_str());
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
		LPANIMATION_SET ani_set_of_obj = nullptr;

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
	CScene::Load();
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
	if (player != nullptr)
		delete player;

	objects.clear();
	units.clear();

	objectsRenderFirst.clear();
	objectsRenderSecond.clear();
	objectsRenderThird.clear();
	
	delete hud;

	current_map = nullptr;
	player = nullptr;
	grid = nullptr;
	hud = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
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
		// On top of pipe
		//mario->SetPosition(2258.0f, 50.0f);
		//On cloudy brick
		//mario->SetPosition(1488.0f, 150.0f);
		mario->SetPosition(673.0f, 350.0f);

	}
	else if (game->IsKeyDown(DIK_Z)) {
		if (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT)) {
			mario->SetIsReadyToRunMax(true);
		}
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}

