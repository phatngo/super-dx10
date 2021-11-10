#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

vector<LPGAMEOBJECT> CScene::GetSceneObjects() {
	vector<LPGAMEOBJECT> obj;
	return obj;
}