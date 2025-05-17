#include "Effect.h"
#include "Debug.h"

int CEffect::GetEffectAniId()
{
	int aniId = -1;
	switch (type)
	{
	case EFFECT_SCORE_100:
		aniId = ID_ANI_SCORE_100; break;
	case EFFECT_SCORE_1000:
		aniId = ID_ANI_SCORE_1000; break;
	default:
		DebugOut(L"NO HAVE ANI ID\n");
		break; // hoặc ID mặc định
	}
	return aniId;
}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x -SCORE_BBOX_WIDTH / 2;
	t = y - SCORE_BBOX_HEIGHT / 2;
	r = l + SCORE_BBOX_WIDTH;
	b = t + SCORE_BBOX_HEIGHT;
}

CEffect::CEffect(float x, float y, int t) : CGameObject(x, y)
{
	this->vy = -SCORE_SPEED_UP;
	this->vx = SCORE_SPEED;
	this->ay = SCORE_GRAVITY;
	this->isLive = true;
	this->time_live_start = GetTickCount64();
	this->type = t;
}

void CEffect::Render()
{
	int aniId = GetEffectAniId();
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

}

void CEffect::Update(DWORD dt)
{

	vy += SCORE_GRAVITY * dt;
	y += vy * dt;
	if(state == EFFECT_SCORE_100 || state == EFFECT_SCORE_1000)
		if (GetTickCount64() - time_live_start > SCORE_LIFETIME) 
		{
			isLive = false;
			vx = vy = 0;
		}
		

}
