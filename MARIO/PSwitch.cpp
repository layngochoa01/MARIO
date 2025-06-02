#include "PSwitch.h"
#include "Debug.h"

void CPSwitch::Render()
{
	if (!CheckObjectInCamera(this)) return;
	if(activated || isFinish) CAnimations::GetInstance()->Get(ANI_ID_PSWITCH_ACTIVE)->Render(x, y + 5.0f);
	else CAnimations::GetInstance()->Get(ANI_ID_PSWITCH_NOT_ACTIVE)->Render(x, y);
	RenderBoundingBox();
}

void CPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera(this)) return;
	if (visible)
	{
		vy = - PSWITCH_SPEED_RISING;  // tiếp tục đi lên
		y += vy * dt;

		if (startY - y >= PSWITCH_NO_ACTIVE_BBOX_HEIGHT)
		{
			y = startY - PSWITCH_NO_ACTIVE_BBOX_HEIGHT;  // giới hạn lên
			SetState(PSWITCH_STATE_IDLE);
		}
	}

	if (activated && !isFinish)
	{
		if (GetTickCount64() - time_live >= TIME_LIVE_PSWITCH_ACTIVE)
		{
			activated = false;
			isFinish = true;
			//SetState(PSWITCH_STATE_IDLE);
			//DebugOut(L"\n============================================\n");
			//DebugOut(L"\n[PSWITCH] PSWICH KHONG CON HOAT DONG\n");
			//DebugOut(L"\n============================================\n");
			
		}
		//DebugOut(L"[PSWITCH ]PS ACTIVE %d, FINISH %d \n\n", IsActivated(), IsFinish());
	}
	//DebugOut(L"[PSWITCH]\t\t");

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
		isFinish = false;
		time_live = GetTickCount64();
		break;
	}
	}
	CGameObject::SetState(s);
}

void  CPSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (activated || isFinish)
	{
		left = x - PSWITCH_BBOX_WIDTH / 2;
		top = y + 4.0f - PSWITCH_ACTIVE_BBOX_HEIGHT / 2;
		right = left + PSWITCH_BBOX_WIDTH;
		bottom = top + PSWITCH_ACTIVE_BBOX_HEIGHT;
		return;
	}
	left = x - PSWITCH_BBOX_WIDTH / 2;
	top = y - PSWITCH_NO_ACTIVE_BBOX_HEIGHT / 2;
	right = left + PSWITCH_BBOX_WIDTH;
	bottom = top + PSWITCH_NO_ACTIVE_BBOX_HEIGHT;
}

void CPSwitch::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}
