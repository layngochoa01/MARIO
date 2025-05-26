#include "PSwitch.h"

void CPSwitch::Render()
{
	if(activated || isFinish) CAnimations::GetInstance()->Get(ANI_ID_PSWITCH_ACTIVE)->Render(x, y);
	else CAnimations::GetInstance()->Get(ANI_ID_PSWITCH_NOT_ACTIVE)->Render(x, y);

}

void CPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += ay * dt;
	

	if (y <= startY - 16.0f) // Trồi lên 1 block
	{
		y = startY - 16.0f;
		vy = 0;
		ay = 0;
		SetState(PSWITCH_STATE_IDLE);
	}

	if (state == PSWITCH_STATE_ACTIVE)
	{
		if (GetTickCount64() - time_live >= TIME_LIVE_PSWITCH_ACTIVE)
		{
			activated = false;
			isFinish = true;
			// TODO: Gọi logic để biến coin trở lại brick nếu cần
		}
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPSwitch::SetState(int s)
{
	switch (s)
	{
	case PSWITCH_STATE_RISING:
	{
		vy = -PSWITCH_SPEED_RISING;
		ay = PSWITCH_GRAVITY;
		visible = true;
		activated = false;
		time_live = 0;
		break;
	}
	case PSWITCH_STATE_IDLE: 
	{
		ay = 0;
		vy = 0;
		visible = false;
		activated = false;
		time_live = 0;
		break;
	}
	case PSWITCH_STATE_ACTIVE:
	{
		ay = 0;
		vy = 0;
		visible = false;
		activated = true;
		time_live = GetTickCount64();
		break;
	}
	}
	CGameObject::SetState(s);
}

void  CPSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PSWITCH_BBOX_WIDTH / 2;
	top = y - PSWITCH_BBOX_HEIGHT / 2;
	right = left + PSWITCH_BBOX_WIDTH;
	bottom = top + PSWITCH_BBOX_HEIGHT;
}

void CPSwitch::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}
