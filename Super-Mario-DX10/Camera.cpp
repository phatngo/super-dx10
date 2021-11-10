#include "Camera.h"
#include "HUD.h"


CCamera* CCamera::__instance = NULL;

CCamera* CCamera::GetInstance() {
	if (__instance == NULL) __instance = new CCamera();
	return __instance;
}

void CCamera::SetCameraPosition(float marioX, int mapWidth) {
	this->mapWidth = mapWidth;
	CMario* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
	if ((int)marioX < playerStartX) {
		cameraPositionX = 0;

	}
	else if ((int)marioX + CGame::GetInstance()->GetScreenWidth() <= cameraFurthestPositionY) {
		cameraPositionX = marioX - playerStartX;

	}
	else {
		cameraPositionX = cameraFurthestPositionY - CGame::GetInstance()->GetScreenWidth() - (cameraFurthestPositionY - mapWidth);
	}
	cameraPositionY = standardCameraPositionY;

	float mx, my;
	player->GetPosition(mx, my);
	isAbove = false;
	if (my < standardCameraPositionY) {
		isAbove = true;
		cameraPositionY = standardCameraPositionY - CGame::GetInstance()->GetScreenHeight() + CAMERA_EXTRA_Y;
	}
	CGame::GetInstance()->SetCamPos(cameraPositionX, cameraPositionY);
}

void CCamera::SetCameraPosition(float marioX) {
	CMario* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
	if ((int)marioX < playerStartX) {
		cameraPositionX = 0;

	}
	else if ((int)marioX + CGame::GetInstance()->GetScreenWidth() <= cameraFurthestPositionY) {
		cameraPositionX = marioX - playerStartX;
	}
	else {

		cameraPositionX = cameraFurthestPositionY - CGame::GetInstance()->GetScreenWidth() - (cameraFurthestPositionY - this->mapWidth);
	}
	cameraPositionY = standardCameraPositionY;

	float mx, my;
	player->GetPosition(mx, my);
	isAbove = false;
	if (my < standardCameraPositionY) {
		isAbove = true;
		cameraPositionY = standardCameraPositionY - CGame::GetInstance()->GetScreenHeight() + CAMERA_EXTRA_Y;
	}
	CGame::GetInstance()->SetCamPos(cameraPositionX, cameraPositionY);
}

D3DXVECTOR3 CCamera::GetPositionInCamera(D3DXVECTOR3 position)
{
	D3DXVECTOR3 pos;
	pos.x = position.x - cameraPositionX;
	pos.y = position.y - cameraPositionY;
	return D3DXVECTOR3(pos.x, pos.y, 0);
}