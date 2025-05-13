#include "BaseMushroom.h"
#include "Mario.h"
#include "Platform.h"
#include "PlayScene.h"
#include "BrickQues.h"



CBaseMushroom::CBaseMushroom(float x, float y, int t) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	this->type = t;
	this->state = MUSHROOM_STATE_IDLE;
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
	vy += ay * dt;
	vx += ax * dt;

	if ( state == MUSHROOM_STATE_COLLECTED){
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBaseMushroom::Render()
{
	int aniId ;
	switch (type) 
	{
	case 1:
		aniId = ID_ANI_RED_MUSHROOM;
		break;
	// có thể mở rộng cho các nấm màu khác
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
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
	else if (dynamic_cast<CBrickQues*>(e->obj))
		OnCollisionWithBrickQuestion(e);
}

void CBaseMushroom::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (platform->IsBlocking()) return;
	if (e->ny < 0) {
		SetPosition(GetX(), platform->GetY() - MUSHROOM_BBOX_HEIGHT);
	}
}

void CBaseMushroom::OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e)
{


}

void CBaseMushroom::SetState(int state)
{
	CGameObject::SetState(state);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state) 
	{
		case MUSHROOM_STATE_IDLE:
			vx = vy = 0;
			break;
		case MUSHROOM_STATE_MOVING:
			vx = (x < mario->GetX()) ? -MUSHROOM_MOVING_SPEED : MUSHROOM_MOVING_SPEED;
			break;
		case MUSHROOM_STATE_COLLECTED:
			vx = 0;
			vy = 0;
			ay = 0;
			break;
	}
}
