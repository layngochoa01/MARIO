#include "Goomba.h"
#include "Platform.h"
#include "Mario.h"
#include "PlayScene.h"

CGoomba::CGoomba(float x, float y, int t):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	this->isUpside = false;
	this->isAttack = false;
	this->isOnGround = false;
	this->type = t;
	
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isUpside) return;
	left = x - GOOMBA_BBOX_WIDTH / 2;
	right = left + GOOMBA_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
	{
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		top = y - GOOMBA_BBOX_HEIGHT/2;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) 
	{
		if (e->nx != 0) 
		{
			vx = -vx;
			CGoomba* goombaOther = dynamic_cast<CGoomba*>(e->obj);
			goombaOther->vx = -goombaOther->vx;

			x += e->nx * 1.0f; // đẩy nhẹ ra ngoài theo hướng va chạm
			goombaOther->x -= e->nx * 1.0f;
		}
		return;
	}

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsGrowing() || mario->IsRaccoon() || mario->GetState() == MARIO_STATE_DIE) return;
	if (!CheckObjectInCamera(this)) return;
	
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	if ((isUpside == true) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT * 6))
	{
		isDeleted = true;
		return;
	}
	else if (type == GOOMBA_TYPE_WING && !isAttack ) 
	{
		ULONGLONG now = GetTickCount64();
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
//
// Get animation ID for GoombaPara
//

int CGoomba::GetAniGoombaPara() {
	int aniId = -1;
	if (isAttack) {
		switch (state)
		{
		case GOOMBA_STATE_UPSIDE:
			aniId = ID_ANI_GOOMBAPARA_UPSIDE; break;
		case GOOMBA_STATE_DIE:
			aniId = ID_ANI_GOOMBAPARA_DIE; break;
		default:
			aniId = ID_ANI_GOOMBAPARA_WALKING;
			break;
		}
	}
	else {
		if (isUpside) {
			aniId = ID_ANI_GOOMBAPARA_UPSIDE;
		}
		else if (!isOnGround) {
			aniId = ID_ANI_GOOMBAPARA_FLY_JUMP;
		}
		else aniId = ID_ANI_GOOMBAPARA_FLY_WALKING;
	}
	return aniId;
}
//
// Get animation ID for GoombaBase
//

int CGoomba::GetAniGoombaBase() {
	switch (state)
	{
	case GOOMBA_STATE_UPSIDE:
		return ID_ANI_GOOMNA_UPSIDE;
	case GOOMBA_STATE_DIE:
		return ID_ANI_GOOMBA_DIE;
	default:
		return ID_ANI_GOOMBA_WALKING;
	}
}


void CGoomba::Render()
{
	if (!CheckObjectInCamera(this)) return;
	int aniId = -1;
	if (type == GOOMBA_TYPE_BASE)
		aniId = GetAniGoombaBase();
	else 
		aniId = GetAniGoombaPara();

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = vy = ay = 0;
			break;
		case GOOMBA_STATE_WALKING:
			vx = -GOOMBA_WALKING_SPEED;
			ay = GOOMBA_GRAVITY;
			isUpside = false;
			isAttack = false;
			die_start = 0;
			break;
		case GOOMBAPARA_STATE_FLY:
			vy = -0.2f; // Nhảy lên
			ay = GOOMBA_GRAVITY;
			break;
		case GOOMBAPARA_STATE_IS_ATTACK:
			vx = -GOOMBA_WALKING_SPEED;
			ay = GOOMBA_GRAVITY;
			isAttack = true;
			break;
		case GOOMBA_STATE_UPSIDE:
			vy = -0.3f;
			ay = GOOMBA_GRAVITY;
			vx = 0.05f;
			isUpside = true;
			die_start = GetTickCount64();
			break;
	}
}
