#include "HUD.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"
#include "Font.h"
#include "Mario.h"
#include "PlayScence.h"
#include "BackUp.h"



HUD::HUD() {
	game_time = TOTAL_GAME_TIME;
	timer.Start();
	isIconPBlinking = false;
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	//Update mario's point
	int totalPoint = BackUp::GetInstance()->GetPoint();
	int i;
	pointDigits.clear();
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
	int totalMoney = BackUp::GetInstance()->GetMoney();
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
	int lives = BackUp::GetInstance()->GetMarioLives();
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
	
	// Update things only happening when in playscene
	if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())) {
		CMario* player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
		//Update game timer
		if (timer.IsStarted() && timer.ElapsedTime() >= TIME_TO_CHANGE_SECOND) {
			if (game_time != 0) {
				game_time--;
			}
			else {
				game_time = TOTAL_GAME_TIME;
			}
			timer.Reset();
			timer.Start();
		}
		//Modify HUD y
		if (CCamera::GetInstance()->IsAbove()) {
			this->y = this->start_Y - CGame::GetInstance()->GetScreenHeight() + CAMERA_EXTRA_Y;
		}
		else {
			this->y = this->start_Y;
		}

		gameTimerDigits.clear();
		for (i = 0; i < TIMER_DIGIT_NUMBER; i++) {
			Font* font;
			if (i == 0) {
				font = new Font(
					firstTimerPositionX,
					this->y + firstTimerPositionY
				);
			}
			else {
				float last_font_X, last_font_Y;
				gameTimerDigits[i - 1]->GetPosition(last_font_X, last_font_Y);
				font = new Font(last_font_X + FONT_WIDTH, last_font_Y);
			}
			gameTimerDigits.push_back(font);
		}
		int time = game_time;
		for (i = TIMER_DIGIT_NUMBER - 1; i >= 0; i--) {
			int digit = time % 10;
			time = time / 10;
			switch (digit)
			{
			case DIGIT_0:
				gameTimerDigits[i]->SetAni(ANI_0);
				break;
			case DIGIT_1:
				gameTimerDigits[i]->SetAni(ANI_1);
				break;
			case DIGIT_2:
				gameTimerDigits[i]->SetAni(ANI_2);
				break;
			case DIGIT_3:
				gameTimerDigits[i]->SetAni(ANI_3);
				break;
			case DIGIT_4:
				gameTimerDigits[i]->SetAni(ANI_4);
				break;
			case DIGIT_5:
				gameTimerDigits[i]->SetAni(ANI_5);
				break;
			case DIGIT_6:
				gameTimerDigits[i]->SetAni(ANI_6);
				break;
			case DIGIT_7:
				gameTimerDigits[i]->SetAni(ANI_7);
				break;
			case DIGIT_8:
				gameTimerDigits[i]->SetAni(ANI_8);
				break;
			case DIGIT_9:
				gameTimerDigits[i]->SetAni(ANI_9);
				break;
			default:
				break;
			}
		}
		//Update mario's power

		if (
			(player->GetSpeedX() != 0 
			&& abs(player->GetSpeedX()) > MARIO_WALKING_SPEED_MAX 
			&& CGame::GetInstance()->IsKeyDown(DIK_A) 
			&& !player->IsBlocked()
			)
			||power != 0) {
			if (player->IsOnGround()) {
				power = (unsigned int)(abs(player->GetSpeedX()) * NUMBER_OF_POWER_ARROW / MARIO_SPEED_RUN_FLY_MAX);
			}
		}
		else {
			power = 0;
		}

		//Update iconP
		if (power == NUMBER_OF_POWER_ARROW) {
			isIconPBlinking = true;
		}
		else {
			isIconPBlinking = false;
			iconPTimerOn.Reset();
			iconPTimerOff.Reset();
		}


		if (isIconPBlinking) {
			if (!iconPTimerOn.IsStarted())
				iconPTimerOn.Start();
			else
				iconPTimerOff.Start();
		}

		if (iconPTimerOn.IsStarted() && iconPTimerOn.ElapsedTime() >= ICON_P_ON_TIME) {
			iconPTimerOn.Reset();
			iconPTimerOff.Start();
		}

		if (iconPTimerOff.IsStarted() && iconPTimerOff.ElapsedTime() >= ICON_P_OFF_TIME) {
			iconPTimerOff.Reset();
			iconPTimerOn.Start();
		}
	}
}

void HUD::Render() {
	animation_set->at(HUD_GENERAL_ANI)->Render(this->x + CCamera::GetInstance()->GetCameraX(),this->y);
	unsigned int i;
	for (i = 0; i < POINT_DIGIT_NUMBER; i++) {
		if (pointDigits.size() != 0) {
			pointDigits[i]->Render();
		}
	}

	for (i = 0; i < moneyDigits.size(); i++) {
		moneyDigits[i]->Render();
	}

	CSprites::GetInstance()->Get(MARIO_ICON_SPRITE)->Draw(
		CCamera::GetInstance()->GetCameraX() + iconMarioPositionX,
		this->y + iconMarioPositionY);

	if(mario_lives!=nullptr)
	  mario_lives->Render();

	if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())) {


		for (i = 0; i < gameTimerDigits.size(); i++) {
			gameTimerDigits[i]->Render();
		}
		for (i = 0; i < power; i++) {
			CSprites::GetInstance()->Get(POWERMELTER_FILLED_ARROW_SPRITE)->Draw(
				CCamera::GetInstance()->GetCameraX() + firstPowerArrowPositionX + (i * ARROW_WIDTH),
				this->y + firstPowerArrowPositionY);
		}
		if (iconPTimerOn.IsStarted() && iconPTimerOn.ElapsedTime() < ICON_P_ON_TIME) {
			CSprites::GetInstance()->Get(POWERMELTER_FILLED_ICONP_SPRITE)->Draw(
				CCamera::GetInstance()->GetCameraX() + iconPPositionX,
				this->y + iconPPositionY);
		}
		
	}

}


