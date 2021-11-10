#include "Card.h"
#include "Utils.h"
#include "PlayScence.h"
#include "CourseClear.h"

void Card::Render()
{
	int ani = CARD_ANI_GENERAL;	
	if (this->currentFrame != -1) {
		switch (this->currentFrame)
		{
		case 0:
			ani = CARD_ANI_MUSHROOM;
			break;
		case 1:
			ani = CARD_ANI_PIRANHA;
			break;
		case 2: 
			ani = CARD_ANI_STAR;
			break;
		default:
			break;
		}
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void Card::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CARD_BBOX_WIDTH;
	b = y + CARD_BBOX_HEIGHT;
}

void Card::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	y += dy;
}

Card::Card() {
	SetState(CARD_STATE_IDLE);
	currentFrame = -1;
}

void Card::SetState(int state) {
	switch (state)
	{
	case CARD_STATE_IDLE:
		vy = 0;
		break;
	case CARD_STATE_FLY_UP:
		currentFrame = animation_set->at(CARD_ANI_GENERAL)->GetCurrentFrame();
		DisplayCourseClear();
		vy = -CARD_FLY_UP_SPEED_Y;
		break;
	default:
		break;
	}
}

void Card::DisplayCourseClear() {
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET tmp_ani_set = animation_sets->Get(COURSE_CLEAR_ANI_SET_ID);
		CourseClear* courseClear = new CourseClear(this->x - COURSE_CLEAR_DX_FROM_CARD, COURSE_CLEAR_Y_COORDINATE);
		courseClear->SetAnimationSet(tmp_ani_set);
		//scene->AddObjects(courseClear);
		scene->GetUnit()->AddUnit(courseClear, scene->GetGrid());

}



