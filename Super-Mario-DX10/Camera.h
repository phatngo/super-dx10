#pragma once
#include "Game.h"


#define CAMERA_EXTRA_Y 40


class CCamera
{

	static CCamera* __instance;
	int cameraPositionY_prescene = 0;
	int cameraPositionX = 0;
	int cameraPositionY = 0;
	float playerStartX = 0.0f;
	int standardCameraPositionY = 0;
	int cameraFurthestPositionY = 0;
	int preScene_standardCameraPositionY = 0;
	int preScene_furthestCameraPositionY = 0;
	bool isAbove = false;
	int mapWidth = 0;
public:
	void SetPreSceneStandardCameraPositionY(int camY) { this->preScene_standardCameraPositionY = camY; }
	int GetPreSceneStandardCameraPositionY() { return this->preScene_standardCameraPositionY; }
	void SetPreSceneCameraFurthestPosition(int camX) { this->preScene_furthestCameraPositionY = camX; }
	int GetPreSceneCameraFurthestPosition() { return this->preScene_furthestCameraPositionY; }
	int GetStandardCameraPositionY() { return this->standardCameraPositionY; }
	int GetCameraFurthestPositionY() { return this->cameraFurthestPositionY; }
	D3DXVECTOR3 GetPositionInCamera(D3DXVECTOR3 position);
	static CCamera* GetInstance();
	void SetCameraPosition(float marioX, int mapWidth);
	void SetStandardCameraPositionY(int standardCameraPositionY) { this->standardCameraPositionY = standardCameraPositionY; }
	void SetCameraFurthestPositionY(int cameraFurthestPositionY) { this->cameraFurthestPositionY = cameraFurthestPositionY; }
	int GetCameraX() { return this->cameraPositionX; }
	int GetCameraY() { return this->cameraPositionY; }
	void SetPlayerStartX(float playerStartX) { this->playerStartX = playerStartX; }
	bool IsAbove() { return this->isAbove; }
	void SetIsAbove(bool b) { this->isAbove = b; }
	void SetCameraPosition(float marioX);
};

