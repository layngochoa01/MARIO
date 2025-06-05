#include "Goomba.h"
#include "Platform.h"
#include "Mario.h"
#include "PlayScene.h"

#include "BrickQues.h"
CGoomba::CGoomba(float x, float y, int t):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	startX = x;
	startY = y;

	this->isDie = false;
	this->isUpside = false;
	this->isJump = false;
	this->isAttack = false;
	this->isOnGround = false;
	this->type = t;
	jumpCount = 0;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isUpside) return;
	if (isDie)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT - 2;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CBrickQues*>(e->obj))
		DebugOut(L"BRICK QUEST \n\n");
	if (!e->obj->IsBlocking()) { return; }
	
	if (dynamic_cast<CGoomba*>(e->obj))
	{
		if (type == GOOMBA_TYPE_WING)
		{
			// Không đổi hướng, xuyên qua Goomba khác
		}
		else
		{
			// Goomba thường đổi hướng khi va chạm nhau
			if (e->nx != 0)
			{
				vx = -vx;
				CGoomba* goombaOther = dynamic_cast<CGoomba*>(e->obj);
				goombaOther->vx = -goombaOther->vx;

				x += e->nx * 1.0f; // đẩy nhẹ ra ngoài theo hướng va chạm
				goombaOther->x -= e->nx * 1.0f;
			}
			//return;
		}
	}

	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) {
			isOnGround = true;
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);

}

void CGoomba::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (e->ny < 0) {
		isOnGround = true;
		SetPosition(GetX(), platform->GetY() - GOOMBA_BBOX_HEIGHT + 1);
	}
}


void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera(this)) return;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsUntouchable() || mario->IsGrowing() || mario->IsTransRaccoon() || mario->GetState() == MARIO_STATE_DIE) return;
	
	vy += ay * dt;
	vx += ax * dt;
	if ( (state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	if ((isUpside == true ) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT ))
	{
		isDeleted = true;
		return;
	}
	if (vy < 0) {
		isOnGround = false;
	}
	if (!isUpside) 
	{
		if ((type == GOOMBA_TYPE_WING) && (!isAttack)) {
			if ((GetTickCount64() - walking_start > TIME_WALKING - TIME_JUMP_SMALL) && !isJump) {
				if (isOnGround && (jumpCount < 3)) {
					vy = -GOOMBA_JUMP_DEFLECT_SPEED / 2;
					jumpCount += 1;
				}
			}
			if (GetTickCount64() - walking_start > TIME_WALKING && !isJump) {
				SetState(GOOMBAPARA_STATE_FLY);

				if ((vx >= 0) && (mario->GetX() < GetX()))
				{
					vx = -GOOMBA_WALKING_SPEED;
				}
				else if ((vx <= 0) && (mario->GetX() > GetX()))
				{
					vx = GOOMBA_WALKING_SPEED;
				}
				walking_start = -1;
				jumpCount = 0;
			}
			else
			{
				if (isJump) {
					SetState(GOOMBA_STATE_WALKING);
				}

			}
		}
		
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
		else 
		{
			if (isUpside) 
			{
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
		DebugOut(L"[GOOMBA] RENDER UPSIDE \n");
		return ID_ANI_GOOMBA_UPSIDE;
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
	else if(type == GOOMBA_TYPE_WING)
		aniId = GetAniGoombaPara();
	else {
		if (aniId == -1)
			DebugOut(L"[GOOMBA ] NOT HAVE ANIID \n");
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	//DebugOut(L"[SetState] Đổi state từ %d sang %d\n", this->state, state);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case GOOMBA_STATE_DIE:
	{
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = vy = ay = 0;
		isDie = true;
		isUpside = false;
		break;
	}
	
	case GOOMBA_STATE_WALKING:
	{
		if (type == GOOMBA_TYPE_BASE)
		{
			if (vx == 0) vx = -GOOMBA_WALKING_SPEED;
			ay = GOOMBA_GRAVITY;
			isDie = false;
			isJump = false;
			isAttack = false;
			isUpside = false;
			walkingCheck = false;
			die_start = 0;
		}
		else
		{
			if (!isAttack)
			{
				isDie = false;
				isJump = false;
				isUpside = false;
				walking_start = GetTickCount64();
			}
			else 
			{
				isDie = false;
				isJump = false;
				isUpside = false;
				walkingCheck = false;
				vx = -GOOMBA_WALKING_SPEED;
				walking_start = GetTickCount64();
			}
		}
		break;
	}
		
	case GOOMBAPARA_STATE_FLY:
	{
		vy = -GOOMBA_JUMP_DEFLECT_SPEED; // Nhảy lên
		isJump = true;
		isOnGround = false;
		break;
	}
			
	case GOOMBAPARA_STATE_IS_ATTACK:
	{
		vx = -GOOMBA_WALKING_SPEED;
		if (type == GOOMBA_TYPE_WING) {
			if (!isAttack) {
				isAttack = true;
				isDie = false;
				isJump = false;
				isUpside = false;
			}
			else {
				vx = 0;
				die_start = GetTickCount64();
				isDie = true;
				isJump = false;
				isUpside = false;
			}
		}
		else {
			vx = 0;
			die_start = GetTickCount64();
			isDie = true;
			isJump = false;
			isUpside = false;
		};
		break;
	}
			
	case GOOMBA_STATE_UPSIDE: 
	{
		vy = -GOOMBA_JUMP_DEFLECT_SPEED;
		isUpside = true;
		isDie = true;
		die_start = GetTickCount64();
		break;
	}		
	}
	CGameObject::SetState(state);
}
