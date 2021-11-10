#include "Portal.h"
#include "Game.h"
#include "Mario.h"


CPortal::CPortal(int scene_id, float start_x, float start_y)
{
	this->scene_id = scene_id;
	this->start_x = start_x;
	this->start_y = start_y;
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	CMario* mario = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	if (mario != NULL)
	{
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom))
			&& mLeft >= oLeft && mRight <= oRight)
		{
				if (CGame::GetInstance()->IsKeyDown(DIK_UP)) {
					mario->SetIsPipe(IS_PIPE_UP);
				}

			    if (CGame::GetInstance()->IsKeyDown(DIK_DOWN)) {
				mario->SetIsPipe(IS_PIPE_DOWN);
			    }
		}
	}
	if (mario->IsPipeUp() || mario->IsPipeDown()) {
		if (this->tag == TO_EXTRA_SCENE) {
			if (mario->GetPipeTimer().IsStarted() && mario->GetPipeTimer().ElapsedTime() >= PIPE_TIME) {
				if (mario->IsPipeUp()) {
					CGame::GetInstance()->SwitchExtraScene(scene_id, start_x, start_y, mario->IsPipeUp());
					mario->SetIsPipe(IS_NOT_PIPE);
				}
				else {
					CGame::GetInstance()->SwitchExtraScene(scene_id, start_x, start_y, !mario->IsPipeUp());
					mario->SetIsPipe(IS_NOT_PIPE);
				}
				//Swith-to-extra
			}
		}
		else if (this->tag == BACK_TO_MAIN_SCENE) {
			if (mario->GetPipeTimer().IsStarted() && mario->GetPipeTimer().ElapsedTime() >= PIPE_TIME) {
				CGame::GetInstance()->SwitchBackToOldScene(scene_id, start_x, start_y, mario->IsPipeUp());
				
			}
		}
	}
}

void CPortal::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	b = y + PORTAL_BBOX_HEIGHT;
	if (scene_id == 4)
		r = x + PORTAL_BBOX_WIDTH_1_3;
	else
		r = x + PORTAL_BBOX_WIDTH;
}