#include "Pipe.h"
void CPipe::Render()
{
	if (!CheckObjectInCamera(this)) return;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PIPE)->Render(x, y);
	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPE_BBOX_WIDTH / 2;
	t = y - PIPE_BBOX_HEIGHT / 2;
	r = l + PIPE_BBOX_WIDTH;
	b = t + PIPE_BBOX_HEIGHT;

}

