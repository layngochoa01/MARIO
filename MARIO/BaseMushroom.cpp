#include "BaseMushroom.h"
#include "BrickQues.h"
#include "Debug.h"

#include "Platform.h"
#include "PlayScene.h"
#include "Mario.h"

CBaseMushroom::CBaseMushroom(float x, float y, int t) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	vy = 0;
	this->type = t;
	startY = y;
	SetState(MUSHROOM_STATE_IDLE);
}

void CBaseMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}


void CBaseMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (isPoppingUp)
	{
		vy = - MUSHROOM_SPEED_RISING;  // tiếp tục đi lên
		y += vy * dt;

		if (startY - y >= MUSHROOM_BBOX_HEIGHT)
		{
			y = startY - MUSHROOM_BBOX_HEIGHT;  // giới hạn lên
			SetState(MUSHROOM_STATE_MOVING);
		}
	}
	else if (state == MUSHROOM_STATE_MOVING)
	{
		vy += ay * dt;
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBaseMushroom::Render()
{
	if (!CheckObjectInCamera(this)) return;
	//DebugOut(L"mushroom type : %d, \n", type);
	switch (type) 
	{
	case MUSHROOM_TYPE_RED:
		CAnimations::GetInstance()->Get(ID_ANI_RED_MUSHROOM)->Render(x, y);
		break;
	// có thể mở rộng cho các nấm màu khác
	}
	
	RenderBoundingBox();
}

void CBaseMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CBaseMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CBaseMushroom*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
}

void CBaseMushroom::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (platform->IsBlocking()) {};
	if (e->ny < 0) {
		SetPosition(GetX(), platform->GetY() - MUSHROOM_BBOX_HEIGHT);
	}
}

#define OFFSET 5
void CBaseMushroom::SetState(int state)
{
	CGameObject::SetState(state);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_RISING:
		vy = -MUSHROOM_SPEED_RISING;
		vx = 0;
		ay = 0;
		isPoppingUp = true;
		startY = y;
		break;
	case MUSHROOM_STATE_MOVING:
		ay = MUSHROOM_GRAVITY;
		vx = ((x + OFFSET) < mario->GetX() ? -MUSHROOM_SPEED : MUSHROOM_SPEED);
		isPoppingUp = false;
		break;

	}

}
