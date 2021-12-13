#include <iostream>
#include <fstream>
#include "IntroScene.h"
#include "Textures.h"
#include "Utils.h"
#include "Brick.h"
#include "IntroGround.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Camera.h"

using namespace std;

CIntroScene::CIntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CIntroSceneKeyHandler(this);
	BackGround = nullptr;
	Three = nullptr;
	Arrow = nullptr;
	//StartScrolling();
}

void CIntroScene::_ParseSection_TEXTURES(string line) {
	CScene::_ParseSection_TEXTURES(line);
}

void CIntroScene::_ParseSection_SPRITES(string line) {
	CScene::_ParseSection_SPRITES(line);
}

void CIntroScene::_ParseSection_ANIMATIONS(string line) {
	CScene::_ParseSection_ANIMATIONS(line);
	if(CAnimations::GetInstance()->Get(800) != NULL)
		Three = CAnimations::GetInstance()->Get(800);
}

void CIntroScene::_ParseSection_ANIMATION_SETS(string line) {
	CScene::_ParseSection_ANIMATION_SETS(line);

	if(CAnimationSets::GetInstance()->Get(ANISET_BACKGROUND_ID)!=NULL)
		BackGround = CAnimationSets::GetInstance()->Get(ANISET_BACKGROUND_ID);
	if (CAnimationSets::GetInstance()->Get(ANISET_ARROW_ID) != NULL)
		Arrow = CAnimationSets::GetInstance()->Get(ANISET_ARROW_ID);
}

void CIntroScene::_ParseSection_OBJECTS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int tag = 0, option_tag_1 = 0, option_tag_2 = 0;
	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());
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
	case OBJECT_TYPE_GROUND:
		obj = new CIntroGround();
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);


	objects.push_back(obj);
}

void CIntroScene::Update(DWORD dt) {
	if (switchTimer.ElapsedTime() >= SWITCH_TIME && switchTimer.IsStarted()) {
		CGame::GetInstance()->SwitchScene(WORLD_SCENE_ID);
	}
}

void CIntroScene::Load() {
	cam = CCamera::GetInstance();
	cam->SetCameraPosition(0.0f, -5.0f);
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;
	DebugOut(L"%d", section);
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[' || line == "") { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CIntroScene::Render() {
	BackGround->at(3)->Render(128.0f, 88.0f);
	Three->Render(THREE_X, THREE_Y);
	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->Render();
	if (switchTimer.IsStarted())
		Arrow->at(0)->Render(ARROW_X, ARROW_Y);
	else
		Arrow->at(1)->Render(ARROW_X, ARROW_Y);
}

void CIntroScene::Unload() {

	for (size_t i = 2; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	BackGround = NULL;
	Arrow = NULL;
	Three = NULL;
	switchTimer.Reset();

	DebugOut(L"Unload Intro Scene\n");
}

void CIntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CIntroScene* intro = ((CIntroScene*)CGame::GetInstance()->GetCurrentScene());
	switch (KeyCode)
	{
	case DIK_RETURN:
		intro->switchTimer.Start();
		DebugOut(L"Enter");
		break;
	default:
		break;
	}
}