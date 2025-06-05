#include "FireBall.h"
#include "Mario.h"
#include "PlayScene.h"
#include <math.h>

void CFireBall::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (dynamic_cast<CPipe*>(e->obj)) return;
	if ( e->obj->IsBlocking()) isDeleted = true;
}

CFireBall::CFireBall(float bx, float by, float targetX, float targetY) : CGameObject(bx, by)
{
	int dirX = (targetX >= bx) ? 1 : -1;
	int dirY = (targetY >= by) ? 1 : -1;

	float length = sqrt(2.0f);
	vx = FIREBALL_SPEED * dirX / length;
	vy = FIREBALL_SPEED * dirY / length;

	ax = 0;
	ay = FIREBALL_GRAVITY;

	direction = (vx > 0) ? 1 : -1;
}


void CFireBall::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIREBALL_BBOX_WIDTH / 2;
	top = y - FIREBALL_BBOX_HEIGH / 2;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGH;
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera(this)) 
	{
		this->isDeleted = true;
		return;
	}
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsGrowing() || mario->IsTransRaccoon() || mario->GetState() == MARIO_STATE_DIE) return;
	vy += ay * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CFireBall::Render()
{
	if (!CheckObjectInCamera(this)) return;
	int aniId = (direction > 0) ? ID_ANI_FIREBALL_RIGHT : ID_ANI_FIREBALL_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();

}
