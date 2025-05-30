#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	switch (type) 
	{
	case BRICK:
		animations->Get(ID_ANI_BRICK)->Render(x, y);
		break;
	case BRICK_DECOR:
		animations->Get(ID_ANI_BRICK_DECOR)->Render(x, y);
		break;
	}
	RenderBoundingBox();
	
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}