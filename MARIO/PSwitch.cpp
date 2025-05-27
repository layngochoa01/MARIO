#include "PSwitch.h"
#include "Debug.h"

void CPSwitch::Render()
{
	if(activated || isFinish) CAnimations::GetInstance()->Get(ANI_ID_PSWITCH_ACTIVE)->Render(x, y + 5.0f);
	else CAnimations::GetInstance()->Get(ANI_ID_PSWITCH_NOT_ACTIVE)->Render(x, y);
}

void CPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	
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

	if (y <= startY - 16.0f) // Trồi lên 1 block
	{
		y = startY - 16.0f;
		vy = 0;
		ay = 0;
		SetState(PSWITCH_STATE_IDLE);
	}
	else 
	{
	
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
	float height;
	if (activated || isFinish) height = PSWITCH_ACTIVE_BBOX_HEIGHT;
	else height = PSWITCH_NO_ACTIVE_BBOX_HEIGHT ;
	left = x - PSWITCH_BBOX_WIDTH / 2;
	top = y - height / 2;
	right = left + PSWITCH_BBOX_WIDTH;
	bottom = top + height;
}

void CPSwitch::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}
