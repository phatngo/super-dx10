#include "Font.h"
#include "Utils.h"
Font::Font(float x, float y) {
	this->SetPosition(x, y);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET tmp_ani_set = animation_sets->Get(FONT_ANI_SET_ID);
	this->SetAnimationSet(tmp_ani_set);
}
void Font::Render() {
	animation_set->at(ani)->Render(CCamera::GetInstance()->GetCameraX() + x, y);
}

void Font::SetAni(int ani) {
	this->ani = ani;
}
