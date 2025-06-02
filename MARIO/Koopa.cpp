#include "Koopa.h"
#include "Goomba.h"

#include "BrickQues.h"
#include "PlantEnemies.h"
#include "FireBall.h"
#include "Mario.h"
#include "Coin.h"
#include "BaseMushroom.h"
#include "Leaf.h"

#include "PlayScene.h"

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
	if (t == KOOPA_TYPE_GREEN_WING) 
	{
		this->isWing = true;
		SetState(KOOPA_STATE_JUMP);
	}
	else 
	{
		isWing = false;
		SetState(KOOPA_STATE_WALKING);
	}
	defend_start = 0;
	comeback_start = 0;
	JumpTime = 0;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera(this)) return;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	vy += ay * dt;
	vx += ax * dt;
	if (isWing && state == KOOPA_STATE_JUMP)
	{
		if (GetTickCount64() - JumpTime > KOOPA_JUMP_RELEASE)
		{
			vy = -KOOPA_JUMP_SPEED;
			JumpTime = GetTickCount64();
		}
	}
	if (!isKicked && isInShell)
	{
		vx = 0;
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
	int aniId = -1;
	switch (type)
	{
	case KOOPA_TYPE_GREEN:
		aniId = GetKoopaGreenAniId(); break;
	case KOOPA_TYPE_RED:
		aniId = GetKoopaRedAniId(); break;
	case KOOPA_TYPE_GREEN_WING:
		aniId = GetKoopaGreenWingAniId(); break;
	default:
		DebugOut(L"[KOOPA RENDER ] NOT FINE ANI ID KOOPA \n"); break;
	}
	
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
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

int CKoopa::GetKoopaGreenAniId()
{
	int aniId = -1;
	if (!isUpset)
	{
		if (state == KOOPA_STATE_WALKING)
			aniId = ((vx > 0) ? ID_ANI_GREEN_WALK_RIGHT : ID_ANI_GREEN_WALK_LEFT);
		else if (state == KOOPA_STATE_SHELL) aniId = ID_ANI_GREEN_SHELL;
		else if (isComeback) aniId = ID_ANI_GREEN_SHELL_BACK;
		else aniId = ID_ANI_GREEN_SHELL_MOVING;
	}
	else
	{
		if (state == KOOPA_STATE_SHELL) aniId = ID_ANI_GREEN_SHELL_UPSET;
		else if (isComeback) aniId = ID_ANI_GREEN_SHELL_UPSET_BACK;
		else aniId = ID_ANI_GREEN_SHELL_UPSET_MOVING;
	}
	return aniId;
}

int CKoopa::GetKoopaGreenWingAniId()
{
	int aniId = -1;
	if (isWing)
	{
		if (state == KOOPA_STATE_JUMP)
			aniId = ((vx > 0) ? ID_ANI_GREEN_WING_JUMP_RIGHT : ID_ANI_GREEN_WING_JUMP_LEFT);
	}
	else
	{
		if (!isUpset)
		{
			if (state == KOOPA_STATE_WALKING)
				aniId = ((vx > 0) ? ID_ANI_GREEN_WALK_RIGHT : ID_ANI_GREEN_WALK_LEFT);
			else if (state == KOOPA_STATE_SHELL) aniId = ID_ANI_GREEN_SHELL;
			else if (isComeback) aniId = ID_ANI_GREEN_SHELL_BACK;
			else aniId = ID_ANI_GREEN_SHELL_MOVING;
		}
		else
		{
			if (state == KOOPA_STATE_SHELL) aniId = ID_ANI_GREEN_SHELL_UPSET;
			else if (isComeback) aniId = ID_ANI_GREEN_SHELL_UPSET_BACK;
			else aniId = ID_ANI_GREEN_SHELL_UPSET_MOVING;
		}
	}
	return aniId;
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking() && !e->obj->IsEnemy()) { return; }
	/*if (!dynamic_cast<CGoomba*>(e->obj)) {
		if (e->ny < 0) {
			
				vy = 0;
				isOnPlatform = true;
		
		}
		if (e->nx != 0 && e->obj->IsBlocking()) {
			vx = -vx;
		}
	}*/
	if (!dynamic_cast<CGoomba*>(e->obj) && !dynamic_cast<CMario*>(e->obj)) {
		if (e->ny < 0)
		{
			if (type != KOOPA_TYPE_GREEN_WING) {
				vy = 0;
				isOnPlatform = true;
			}
			else {
				if (state == KOOPA_STATE_JUMP) {
					vy = -KOOPA_JUMP_SPEED;
				}
				else vy = 0;
			}
		}
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = -vx;
		}
	}


	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBrickQues*>(e->obj))
		OnCollisionWithBrickQues(e);
	else if (dynamic_cast<CBrickPSwitch*>(e->obj))
		OnCollisionWithBrickPSwitch(e);
	else if (dynamic_cast<CFireBall*>(e->obj))
		OnCollisionWithFireBall(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);

	
}

void CKoopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	//DebugOut(L"[KOOPA GET BBOX] IS BBOX , TYPE %d, ISWING %d\n", type, isWing);
	if (isWing)
	{
		l = x - KOOPA_GREEN_WING_BBOX_WIDTH / 2;
		t = y - KOOPA_GREEN_WING_BBOX_HEIGHT / 2;
		r = l + KOOPA_GREEN_WING_BBOX_WIDTH;
		b = t + KOOPA_GREEN_WING_BBOX_HEIGHT;
	}
	else if (state == KOOPA_STATE_WALKING) 
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

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	DebugOut(L"[KOOPA] COLLISION WITH GOOMBA\n");
	if (isKicked) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->SetScore(mario->GetScore() + SCORE_100);
		mario->AddScoreEffect(e->obj->GetX(), e->obj->GetY(), SCORE_100);
		goomba->SetState(GOOMBA_STATE_UPSIDE);
	}
}

void CKoopa::OnCollisionWithBrickQues(LPCOLLISIONEVENT e)
{
	DebugOut(L"ĐANG VA CHẠM VỚI BRICKQUES \n\n");
	CBrickQues* questionBrick = dynamic_cast<CBrickQues*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!questionBrick || questionBrick->GetIsUnbox() || questionBrick->GetIsEmpty())
		return;
	

	if (e->nx != 0 && isKicked) {
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		float xT, yT;
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

	//DebugOut(L"ĐANG VA CHẠM VỚI BRICKPSWITCH \n\n");
	CBrickPSwitch* brick = dynamic_cast<CBrickPSwitch*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!brick || (brick->GetModel() == MODEL_PSWITCH && brick->IsEmpty()))
		return;

	if (e->nx != 0 && isKicked )
	{
		if (brick->GetModel() == MODEL_PSWITCH) 
		{
			brick->SetState(BRICK_STATE_NO_PSWITCH);
			brick->CreatePSwitch();
		}
		else 
		{
			brick->SetState(BRICK_STATE_BREAK);
		}
		
	}

	
	if (!brick) return;
	if (e->ny < 0) 
	{
		vy = 0;
		isOnPlatform = true;

		if (state == KOOPA_STATE_WALKING && vx == 0)
		{
			vx = (type == KOOPA_TYPE_RED) ? -KOOPA_WALK_SPEED : KOOPA_WALK_SPEED;
		}
		if ((type == KOOPA_TYPE_RED) && (state == KOOPA_STATE_WALKING))
		{
			UpdateWalkingOnBrickPSwitch(brick);
		}
	}
	
}

void CKoopa::OnCollisionWithFireBall(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	SetState(KOOPA_STATE_DIE);
}

void CKoopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	//DebugOut(L"ĐANG VA CHẠM VỚI PLATFORM \n\n");
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (e->ny < 0)
	{
		vy = 0;
		isOnPlatform = true;


		if (state == KOOPA_STATE_WALKING && vx == 0)
		{
			vx = (type == KOOPA_TYPE_RED) ? -KOOPA_WALK_SPEED : KOOPA_WALK_SPEED;
		}
		if ((type == KOOPA_TYPE_RED) && (state == KOOPA_STATE_WALKING))
		{
			UpdateWalkingOnPlatform(platform);
		}
	}
	
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{

}

void CKoopa::OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e)
{
	if (isKicked) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->SetScore(mario->GetScore() + SCORE_100);
		mario->AddScoreEffect(e->obj->GetX(), e->obj->GetY(), SCORE_100);
		e->obj->Delete();
	}
}

void CKoopa::SetState(int state)
{
	if (this->state == KOOPA_STATE_DIE) return;
	switch (state)
	{
	case KOOPA_STATE_WALKING:
	{
		if(vx == 0.0f ) vx = -KOOPA_WALK_SPEED;
		vy = 0;
		ay = KOOPA_GRAVITY;
		isWing = false;
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
		ax = 0;
		ay = KOOPA_GRAVITY;
		isInShell = true;
		isKicked = false;
		isComeback = false;
		isHeld = false;
		//if (isOnPlatform) vx = 0;
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

	case KOOPA_STATE_JUMP:
	{
		isUpset = false;
		isWing = true;
		isComeback = false;
		isKicked = false;
		vx = -KOOPA_WALK_SPEED;
		if (isWing) {
			ay = KOOPA_GRAVITY_WING;
			//vy = -KOOPA_JUMP_SPEED;
		}
		break;
	}
	}
	CGameObject::SetState(state);
}

void CKoopa::UpdateWalkingOnPlatform(CPlatform* platform)
{
	float leftBound = platform->GetX();
	float rightBound = (platform->GetX() + platform->GetCellWidth() * platform->GetLength() - KOOPA_BBOX_WIDTH / 2);
	UpdateWalkingOnEdge(leftBound, rightBound);
}
// chỉ mới 1 ô
void CKoopa::UpdateWalkingOnBrickPSwitch(CBrickPSwitch* brick)
{
	float leftBound = brick->GetX() - BRICK_MODEL_BBOX_HEIGHT / 2 + 3.0f;
	float rightBound = leftBound + BRICK_MODEL_BBOX_HEIGHT - 5.0f;
	UpdateWalkingOnEdge(leftBound, rightBound);
}

void CKoopa::UpdateWalkingOnEdge(float leftBound, float rightBound)
{
	if ((type == KOOPA_TYPE_RED) && (state == KOOPA_STATE_WALKING)) 
	{
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
