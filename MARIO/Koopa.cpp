#include "Koopa.h"

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CKoopa::Render()
{
}

int CKoopa::GetKoopaRedAniId()
{
	if (!isUpset) 
	{
		if (state == KOOPA_STATE_WALKING)
			return ((vx > 0) ? ID_ANI_RED_WALK_RIGHT : ID_ANI_RED_WALK_LEFT);
		else if (state == KOOPA_STATE_SHELL) return ID_ANI_RED_SHELL;
		else if (state == KOOPA_STATE_COMEBACK) return ID_ANI_RED_SHELL_BACK;
		else return ID_ANI_RED_SHELL_MOVING;
	}
	else 
	{
		if (state == KOOPA_STATE_SHELL) return ID_ANI_RED_SHELL_UPSET;
		else if (state == KOOPA_STATE_COMEBACK) return ID_ANI_RED_SHELL_UPSET_BACK;
		else return ID_ANI_RED_SHELL_UPSET_MOVING;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CKoopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == KOOPA_STATE_WALKING) 
	{
		l = x - KOOPA_BBOX_WIDTH / 2;
		t = y - KOOPA_BBOX_HEIGHT / 2;
		r = l + KOOPA_BBOX_WIDTH;
		b = t + KOOPA_BBOX_HEIGHT;
	}
	else if (state != KOOPA_STATE_DIE)
	{
		l = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		t = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		r = l + KOOPA_SHELL_BBOX_WIDTH;
		b = t + KOOPA_SHELL_BBOX_HEIGHT;
	}
}

CKoopa::CKoopa(float x, float y, int t) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	this->type = t;
	this->isHeld = false;
	this->isInShell = false;
	this->isKicked = false;
	defend_start = 0;
}

void CKoopa::SetState(int state)
{
}
