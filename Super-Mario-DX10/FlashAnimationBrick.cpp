#include "FlashAnimationBrick.h"
#include "Game.h"
#include "PlayScence.h"
#include "Piece.h"
#include "FireBullet.h"

void CFlashAnimationBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isDestroyed)
		return;
	CGameObject::Update(dt, coObjects);
	if(CGame::GetInstance()->IsKeyDown(DIK_P)) {
		if (!transformToCoinTimer.IsStarted()) {
			transformToCoinTimer.Start();
			SetState(COIN_STATE);

			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET tmp_ani_set = animation_sets->Get(COIN_ANI_SET_ID);
			this->SetAnimationSet(tmp_ani_set);
		}
	}
	if (transformToCoinTimer.IsStarted() && transformToCoinTimer.ElapsedTime() >= DISPLAY_COIN_TIME) {
		transformToCoinTimer.Reset();
		this->SetAnimationSet(this->GetOriginalAniSet());
		SetState(FLASH_BRICK_STATE_FLASHING);
		
	}
}

void CFlashAnimationBrick::Render() {
	if (isDestroyed)
		return;
	if (this->state == FLASH_BRICK_STATE_FLASHING || this->state == COIN_STATE) {
			animation_set->at(FLASH_BRICK_ANI_GENERAL)->Render(x, y);
			RenderBoundingBox();
	}
}

CFlashAnimationBrick::CFlashAnimationBrick() {
	SetState(FLASH_BRICK_STATE_FLASHING);
}

void CFlashAnimationBrick::SetState(int state) {
	switch (state)
	{
	case FLASH_BRICK_STATE_NON_EXIST:
		CreatePieces();
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CFlashAnimationBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isDestroyed) {
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
}

void CFlashAnimationBrick::CreatePieces() {
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET tmp_ani_set = animation_sets->Get(PIECE_ANI_SET_ID);

	CPiece* piece_left_up = new CPiece(PIECE_LEFT, PIECE_UP);
	piece_left_up->SetPosition(x, y);
	piece_left_up->SetAnimationSet(tmp_ani_set);

	CPiece* piece_left_down = new CPiece(PIECE_LEFT, PIECE_DOWN);
	piece_left_down->SetPosition(x, y + PIECE_BBOX_HEIGHT);
	piece_left_down->SetAnimationSet(tmp_ani_set);

	CPiece* piece_right_up = new CPiece(PIECE_RIGHT, PIECE_UP);
	piece_right_up->SetPosition(x+PIECE_BBOX_WIDTH, y);
	piece_right_up->SetAnimationSet(tmp_ani_set);

	CPiece* piece_right_down = new CPiece(PIECE_RIGHT, PIECE_DOWN);
	piece_right_down->SetPosition(x + PIECE_BBOX_WIDTH, y + PIECE_BBOX_HEIGHT);
	piece_right_down->SetAnimationSet(tmp_ani_set);

	scene->GetUnit()->AddUnit(piece_left_up, scene->GetGrid());
	scene->GetUnit()->AddUnit(piece_left_down, scene->GetGrid());
	scene->GetUnit()->AddUnit(piece_right_up, scene->GetGrid());
	scene->GetUnit()->AddUnit(piece_right_down, scene->GetGrid());
}

