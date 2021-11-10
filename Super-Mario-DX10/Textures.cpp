#include <Windows.h>

#include <d3d10.h>
#include <d3dx10.h>

#include "Utils.h"
#include "Game.h"
#include "Textures.h"

CTextures * CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures *CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(int id, LPCWSTR filePath)
{
	textures[id] = CGame::GetInstance()->LoadTexture(filePath);
}

LPTEXTURE CTextures::Get(unsigned int id)
{
	return textures[id];
}

/*
	Clear all loaded textures
*/
void CTextures::Clear()
{
	for (auto x : textures)
	{
		LPTEXTURE tex = x.second;
		if (tex!=NULL) delete tex;
	}
	
	textures.clear();
}



