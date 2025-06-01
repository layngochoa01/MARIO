#include "Coin.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
void CCoin::Render()
{
	if (!IsVisible()) return;
	if (!CheckObjectInCamera(this)) return;
	CAnimations* animations = CAnimations::GetInstance();
	if (isCoinSwitch) 
	{
		animations->Get(ID_ANI_COIN_SWITCH)->Render(x, y);
	}
	else animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!IsVisible()) return;
	if (!CheckObjectInCamera(this)) return;
	if (!canCollect)
	{
		vy += ay * dt;
		if (vy > COIN_SPEED_FALL) {
			Delete();
		}
	}
	else vy = vx = 0;
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CCoin::SetState(int s)
{
	switch (s) {
	case COIN_SUM:
		ay = COIN_GRAVITY;
		vy = -COIN_SPEEP_UP;
		canCollect = false;
		break;

	case COIN_NOT_SUM:
		vy = vx = 0;
		ay = 0;
		canCollect = true;
		break;
	case COIN_SWITCH_NOT_SUM:
		vy = vx = 0;
		ay = 0;
		canCollect = true;
		isCoinSwitch = true;
	}
	CGameObject::SetState(s);
}
