#include "CourseClear.h"

void CourseClear::Render() {
	animation_set->at(COURSE_CLEAR_ANI_GENERAL)->Render(x,y);
}

CourseClear::CourseClear(float x, float y) {
	CGameObject::SetPosition(x, y);
}