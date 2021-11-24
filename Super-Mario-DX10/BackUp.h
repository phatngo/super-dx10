#pragma once
#include "Utils.h"

class BackUp
{
private:
	int point = 0;
	int money = 0;
	int level = 1;
	float worldplayer_x = 0.0f;
	float worldplayer_y = 0.0f;
	static BackUp* __instance;
public:
	static BackUp* GetInstance();
	void SetPoint(int point) { this->point = point; }
	void SetMoney(int money) { this->money = money; }
	int GetPoint() { return this->point; }
	int GetMoney() { return this->money; }
	void SetMarioLevel(int marioLevel) { this->level = marioLevel; }
	int GetMarioLevel() { return this->level; }
	void SetWorldPlayerPostion(float x, float y) { worldplayer_x = x; worldplayer_y = y; }
	void GetWorldPlayerPostion(float& x, float& y) { x = worldplayer_x; y = worldplayer_y; }
};

