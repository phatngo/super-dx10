#include "HUD.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"
#include "Font.h"
#include "Mario.h"



HUD::HUD() {

}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//Modify HUD y
	if (CCamera::GetInstance()->IsAbove()) {
		this->y = this->start_Y - CGame::GetInstance()->GetScreenHeight() + CAMERA_EXTRA_Y;
	}
	else {
		this->y = this->start_Y;
	}

	CMario* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();

	// Update Mario's point
	pointDigits.clear();
	//Mario's point update
	int totalPoint = player->GetPoint();
	int i;
	for (i = 0; i < POINT_DIGIT_NUMBER; i++) {
		Font* font;
		if (i == 0) {
			font = new Font(
				firstPointPositionX, 
				this->y + firstPointPositionY
			);
		}
		else {
			float last_font_X, last_font_Y;
			pointDigits[i-1]->GetPosition(last_font_X, last_font_Y);
			font = new Font(last_font_X + FONT_WIDTH, last_font_Y);
		}
		pointDigits.push_back(font);
	}
	
	for (i = POINT_DIGIT_NUMBER - 1 ; i >= 0; i--) {
		int digit = totalPoint % 10;
		totalPoint = totalPoint / 10;
		switch (digit)
		{
		case DIGIT_0:
			pointDigits[i]->SetAni(ANI_0);
			break;
		case DIGIT_1:
			pointDigits[i]->SetAni(ANI_1);
			break;
		case DIGIT_2:
			pointDigits[i]->SetAni(ANI_2);
			break;
		case DIGIT_3:
			pointDigits[i]->SetAni(ANI_3);
			break;
		case DIGIT_4:
			pointDigits[i]->SetAni(ANI_4);
			break;
		case DIGIT_5:
			pointDigits[i]->SetAni(ANI_5);
			break;
		case DIGIT_6:
			pointDigits[i]->SetAni(ANI_6);
			break;
		case DIGIT_7:
			pointDigits[i]->SetAni(ANI_7);
			break;
		case DIGIT_8:
			pointDigits[i]->SetAni(ANI_8);
			break;
		case DIGIT_9:
			pointDigits[i]->SetAni(ANI_9);
			break;
		default:
			break;
		}
	}

	// Update Mario's money
	moneyDigits.clear();
	i = 0;
	int totalMoney = player->GetMoney();
	while (totalMoney / 10 != 0) {
		int digit = totalMoney % 10;
		totalMoney = totalMoney / 10;
		Font* font;
		if (i == 0) {
			font = new Font(
				lastMoneyPositionX,
				this->y + lastMoneyPositionY
			);
		}
		else {
			float last_font_X, last_font_Y;
			moneyDigits[i - 1]->GetPosition(last_font_X, last_font_Y);
			font = new Font(
				last_font_X - FONT_WIDTH,
				this->start_Y + lastMoneyPositionY
			);
		}
		switch (digit)
		{
		case DIGIT_0:
			font->SetAni(ANI_0);
			break;
		case DIGIT_1:
			font->SetAni(ANI_1);
			break;
		case DIGIT_2:
			font->SetAni(ANI_2);
			break;
		case DIGIT_3:
			font->SetAni(ANI_3);
			break;
		case DIGIT_4:
			font->SetAni(ANI_4);
			break;
		case DIGIT_5:
			font->SetAni(ANI_5);
			break;
		case DIGIT_6:
			font->SetAni(ANI_6);
			break;
		case DIGIT_7:
			font->SetAni(ANI_7);
			break;
		case DIGIT_8:
			font->SetAni(ANI_8);
			break;
		case DIGIT_9:
			font->SetAni(ANI_9);
			break;
		default:
			break;
		}
		moneyDigits.push_back(font);
		i++;
	}
	Font* font;
	if (moneyDigits.size() == 0) {
		font = new Font(
			lastMoneyPositionX,
			this->y + lastMoneyPositionY
		);
		switch (totalMoney)
		{
		case DIGIT_0:
			font->SetAni(ANI_0);
			break;
		case DIGIT_1:
			font->SetAni(ANI_1);
			break;
		case DIGIT_2:
			font->SetAni(ANI_2);
			break;
		case DIGIT_3:
			font->SetAni(ANI_3);
			break;
		case DIGIT_4:
			font->SetAni(ANI_4);
			break;
		case DIGIT_5:
			font->SetAni(ANI_5);
			break;
		case DIGIT_6:
			font->SetAni(ANI_6);
			break;
		case DIGIT_7:
			font->SetAni(ANI_7);
			break;
		case DIGIT_8:
			font->SetAni(ANI_8);
			break;
		case DIGIT_9:
			font->SetAni(ANI_9);
			break;
		default:
			break;
		}
	}
	else {
		float last_font_X, last_font_Y;
		moneyDigits[moneyDigits.size() - 1]->GetPosition(last_font_X, last_font_Y);
		font = new Font(last_font_X - FONT_WIDTH,
			this->y + lastMoneyPositionY);
		switch (totalMoney)
		{
		case DIGIT_0:
			font->SetAni(ANI_0);
			break;
		case DIGIT_1:
			font->SetAni(ANI_1);
			break;
		case DIGIT_2:
			font->SetAni(ANI_2);
			break;
		case DIGIT_3:
			font->SetAni(ANI_3);
			break;
		case DIGIT_4:
			font->SetAni(ANI_4);
			break;
		case DIGIT_5:
			font->SetAni(ANI_5);
			break;
		case DIGIT_6:
			font->SetAni(ANI_6);
			break;
		case DIGIT_7:
			font->SetAni(ANI_7);
			break;
		case DIGIT_8:
			font->SetAni(ANI_8);
			break;
		case DIGIT_9:
			font->SetAni(ANI_9);
			break;
		default:
			break;
		}
	}
	moneyDigits.push_back(font);
	
	// Update Mario's lives
	mario_lives = new Font(
		livePositionX,
		this->y + livePositionY
	);
	int lives = player->GetMarioLives();
	switch (lives)
	{
	case DIGIT_0:
		mario_lives->SetAni(ANI_0);
		break;
	case DIGIT_1:
		mario_lives->SetAni(ANI_1);
		break;
	case DIGIT_2:
		mario_lives->SetAni(ANI_2);
		break;
	case DIGIT_3:
		mario_lives->SetAni(ANI_3);
		break;
	case DIGIT_4:
		mario_lives->SetAni(ANI_4);
		break;
	default:
		break;
	}
}

void HUD::Render() {
	animation_set->at(0)->Render(this->x + CCamera::GetInstance()->GetCameraX(),this->y);

	for (int i = 0; i < POINT_DIGIT_NUMBER; i++) {
		pointDigits[i]->Render();
	}

	for (int i = 0; i < moneyDigits.size(); i++) {
		moneyDigits[i]->Render();
	}
	mario_lives->Render();
}


