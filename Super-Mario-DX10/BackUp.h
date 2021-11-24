#pragma once

class BackUp
{
private:
	int point = 0;
	int money = 0;
	int level = 1;
	static BackUp* __instance;
public:
	static BackUp* GetInstance();
	void SetPoint(int point) { this->point = point; }
	void SetMoney(int money) { this->money = money; }
	int GetPoint() { return this->point; }
	int GetMoney() { return this->money; }
	void SetMarioLevel(int marioLevel) { this->level = marioLevel; }
	int GetMarioLevel() { return this->level; }
};

