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
	l = r = t = b = 0;
	
}

void CEffect::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

CEffect::CEffect(float x, float y, int t) : CGameObject(x, y)
{
	this->vy = -SCORE_SPEED_UP;
	this->ay = SCORE_GRAVITY;
	this->isLive = true;
	this->time_live_start = GetTickCount64();;
	this->type = t;
}

void CEffect::Render()
{
	int aniId = GetEffectAniId();
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += ay * dt;
	if (isLive) 
	{
		if (GetTickCount64() - time_live_start > SCORE_LIFETIME)
		{
			isLive = false;
			vx = vy = 0;
			Delete();
			
		}
		else vy = -SCORE_SPEED_UP;
	}
	//DebugOut(L"vx %f , vy %f \n\n", vx, vy);
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}
