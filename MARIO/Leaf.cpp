#include "Leaf.h"
#include "Utils.h"
#include "Debug.h"
#include "Platform.h"
#include "Brick.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = LEAF_GRAVITY;
	this->isCollected = false;
	this->sway_start = 0;
	this->direction = 1;
	SetState(LEAF_STATE_IDLE);
	isOnPlatform = false;

}

void CLeaf::Render()
{
	if (!CheckObjectInCamera(this)) return;
	int aniId = -1;
	aniId = (vx <= 0 ? ID_ANI_LEAF_RIGHT : ID_ANI_LEAF_LEFT);
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera(this)) return;
	if (isOnPlatform) 
	{
		vx = 0; vy = 0;
	}
	else 
	{
		vy += ay * dt;
		vx += ax * dt;
		if (state == LEAF_STATE_FALL)
		{
			if ((GetTickCount64() - sway_start) > LEAF_TIME_SWAY)
			{
				direction = -direction;
				vx = direction * LEAF_SPEED_SWAY;
				ax = -ax;
				sway_start = GetTickCount64();
			}
		}
		else if (state == LEAF_STATE_RISE && vy > 0)
		{
			SetState(LEAF_STATE_FALL);
		}

	}
	
	CGameObject::Update(dt, coObjects); 
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CLeaf*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) 
		{
			SetPosition(GetX(), e->obj->GetY() - LEAF_BBOX_HEIGHT);
			vx = 0; 
			vy = 0;
			isOnPlatform = true;
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

}

void CLeaf::SetState(int s)
{
	CGameObject::SetState(s);
	switch (s) 
	{
	case LEAF_STATE_RISE:
		vy = -LEAF_SPEEP_RISING;
		vx = 0;
		ay = LEAF_GRAVITY;
		isCollected = true;
		break;
	case LEAF_STATE_FALL:
		vx = direction * LEAF_SPEED_SWAY;
		ay = LEAF_SPEED_FALL;
		ax += (direction * 0.0001f);
		sway_start = GetTickCount64();
		break;
	}
}
