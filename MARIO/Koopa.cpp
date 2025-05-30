#include "Koopa.h"
#include "Goomba.h"

#include "BrickQues.h"
#include "BrickPSwitch.h"
#include "FireBall.h"
#include "Mario.h"
#include "Coin.h"
#include "BaseMushroom.h"
#include "Leaf.h"

#include "PlayScene.h"

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera(this)) return;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	vy += ay * dt;

	vx += ax * dt;
	
	if (!isKicked && isInShell)
	{
		if (GetTickCount64() - defend_start > KOOPA_COMEBACK_TIME) {
			isComeback = true;
		}

		if (GetTickCount64() - defend_start > KOOPA_DEFEND_TIMEOUT) {
			if (isComeback) {
				SetState(KOOPA_STATE_WALKING);
				vy = -0.1f; // Nhảy nhẹ khi hồi phục
				defend_start = 0;
				isComeback = false;
			}
		}
	}
	if (isTurning) {
		if (GetTickCount64() - turnaround_delay >= KOOPA_TIME_DELAY_AROUND)
		{
			//vx = -vx;
			isTurning = false;
		}
		else { 
			if (vx > 0) vx = KOOPA_WALK_SPEED;
			else vx = -KOOPA_WALK_SPEED;
		}
	}
	
	if (isUpset && !isOnPlatform) {
		vx = -KOOPA_WALK_SPEED;
	}

	if (state == KOOPA_STATE_DIE)
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	if (!CheckObjectInCamera(this)) return;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(GetKoopaRedAniId())->Render(x, y);
	//RenderBoundingBox();
}

int CKoopa::GetKoopaRedAniId()
{
	if (!isUpset) 
	{
		if (state == KOOPA_STATE_WALKING)
			return ((vx > 0) ? ID_ANI_RED_WALK_RIGHT : ID_ANI_RED_WALK_LEFT);
		else if (state == KOOPA_STATE_SHELL) return ID_ANI_RED_SHELL;
		else if (isComeback) return ID_ANI_RED_SHELL_BACK;
		else return ID_ANI_RED_SHELL_MOVING;
	}
	else 
	{
		if (state == KOOPA_STATE_SHELL) return ID_ANI_RED_SHELL_UPSET;
		else if (isComeback) return ID_ANI_RED_SHELL_UPSET_BACK;
		else return ID_ANI_RED_SHELL_UPSET_MOVING;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) { return; }
	if (!dynamic_cast<CGoomba*>(e->obj)) {
		if (e->ny < 0) {
			
				vy = 0;
				isOnPlatform = true;
		
		}
		if (e->nx != 0 && e->obj->IsBlocking()) {
			vx = -vx;
		}
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBrickQues*>(e->obj))
		OnCollisionWithBrickQues(e);
	else if (dynamic_cast<CFireBall*>(e->obj))
		OnCollisionWithFireBall(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);

	
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
	this->vx = 0;
	this->vy = 0;
	this->type = t;
	this->isInShell = false;
	this->isUpset = false;
	this->isHeld = false;
	this->isComeback = false;
	this->isOnPlatform = false;
	this->isKicked = false;
	defend_start = 0;
	comeback_start = 0;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (isKicked) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->SetScore(mario->GetScore() + SCORE_100);
		goomba->SetState(GOOMBA_STATE_DIE);
	}
}

void CKoopa::OnCollisionWithBrickQues(LPCOLLISIONEVENT e)
{
	
	CBrickQues* questionBrick = dynamic_cast<CBrickQues*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!questionBrick || questionBrick->GetIsUnbox() || questionBrick->GetIsEmpty())
		return;
	

	if (e->nx != 0 && isKicked) {
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		float xT, yT, minY;
		questionBrick->GetPosition(xT, yT);

		if (questionBrick->GetItemType() == BRICK_QUES_COIN)
		{
			mario->SetCoin(mario->GetCoin() + 1);
			CCoin* coin = new CCoin(xT, yT);
			coin->SetState(COIN_SUM);
			scene->PushObject(coin);
			questionBrick->SetIsEmpty(true);
		}
		else if (questionBrick->GetItemType() == BRICK_QUES_MUSHROOM_RED_OR_LEAF)
		{
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				CBaseMushroom* mushroom = new CBaseMushroom(xT, yT, MUSHROOM_TYPE_RED);
				mushroom->SetState(MUSHROOM_STATE_RISING);
				scene->PushObject(mushroom);
				questionBrick->SetIsEmpty(true);
			}
			else if (mario->GetLevel() >= MARIO_LEVEL_BIG)
			{
				CLeaf* leaf = new CLeaf(xT, yT);
				leaf->SetState(LEAF_STATE_RISE);
				scene->PushObject(leaf);
				questionBrick->SetIsEmpty(true);
			}
		}
	}
	

}

void CKoopa::OnCollisionWithBrickPSwitch(LPCOLLISIONEVENT e)
{

}

void CKoopa::OnCollisionWithFireBall(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	SetState(KOOPA_STATE_DIE);
}

void CKoopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (e->ny < 0)
	{
		vy = 0;
		isOnPlatform = true;
		

		if (state == KOOPA_STATE_WALKING && vx == 0)
		{
			vx = (type == KOOPA_TYPE_RED) ? -KOOPA_WALK_SPEED : KOOPA_WALK_SPEED;
		}
	}
	UpdateWalkingOnPlatform(platform);

}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{

}

void CKoopa::SetState(int state)
{
	if (this->state == KOOPA_STATE_DIE) return;
	switch (state)
	{
	case KOOPA_STATE_WALKING:
	{
		if(vx == 0.0f )vx = -KOOPA_WALK_SPEED;
		vy = 0;
		ay = KOOPA_GRAVITY;
		isComeback = false;
		isUpset = false;//upset chỉ true khi raccoon quét đuôi
		isInShell = false;
		isHeld = false;
		isKicked = false;
		isComeback = false;
		break;
	}
	case KOOPA_STATE_SHELL:
	{
		vx = vy = 0;
		ay = KOOPA_GRAVITY;
		isInShell = true;
		isComeback = false;
		isHeld = false;
		if (isOnPlatform) vx = 0;
		defend_start = GetTickCount64();
		break;
	}
	case KOOPA_STATE_SHELL_MOVING:
	{
		if (isHeld) {
			vy = - 0.04f ;
		}
		ay = KOOPA_GRAVITY;
		vx = vx * KOOPA_KICKED_SPEED;
		//hướng của vx đã được set up khi mario đá shell
		isOnPlatform = true;
		isKicked = true;
		isHeld = false;
		break;
	}
	}
	CGameObject::SetState(state);
}

void CKoopa::UpdateWalkingOnPlatform(CPlatform* platform) 
{
	if ((type == KOOPA_TYPE_RED) && (state == KOOPA_STATE_WALKING)) 
	{
		float leftBound = platform->GetX() ;
		float rightBound = (platform->GetX() + platform->GetCellWidth() * platform->GetLength() - KOOPA_BBOX_WIDTH / 2);
		if (GetX() <= leftBound) {
			x = leftBound;
			isTurning = true;
			turnaround_delay = GetTickCount64();
			vx = KOOPA_WALK_SPEED; // Quay đầu sang phải
		}
		else if (GetX() >= rightBound) {
			x = rightBound;
			isTurning = true;
			turnaround_delay = GetTickCount64();
			vx = -KOOPA_WALK_SPEED; // Quay đầu sang trái
		}

		
	}
}
