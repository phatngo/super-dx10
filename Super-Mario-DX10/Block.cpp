#include "Block.h"

void CBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BLOCK_BBOX_WIDTH;
	bottom = y + BLOCK_BBOX_HEIGHT;
}


void CBlock::Render()
{
	RenderBoundingBox();
}

