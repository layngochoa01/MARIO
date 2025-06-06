#include "TailHitbox.h"
#include "debug.h"
#include "Game.h" 

#include "Goomba.h"
#include "Koopa.h"
#include "BrickQues.h"
#include "BrickPSwitch.h"
#include "PlantEnemies.h"
#include "Effect.h"
#include "Collision.h"
#include "PlayScene.h"

#include "Coin.h"
#include "BaseMushroom.h"
#include "Effect.h"
#include "Leaf.h"
CTailHitbox::CTailHitbox(float x, float y)
{
	this->x = x;
	this->y = y;
	createTime = GetTickCount64();
}

void CTailHitbox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - createTime > TAIL_HITBOX_LIFE_TIME)
	{
		this->Delete();
		return;
	}

	CGameObject::Update(dt, coObjects);

	// Xử lý va chạm động (Swept AABB)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CCollision::GetInstance()->Scan(this, dt, coObjects, coEvents);

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		if (coEvents[i]->WasCollided())
			OnCollisionWith(coEvents[i]);
		delete coEvents[i];
	}

	// Kiểm tra va chạm tĩnh (AABB) thủ công
	for (LPGAMEOBJECT obj : *coObjects)
	{
		if (obj->IsDeleted()) continue;

		float l1, t1, r1, b1;
		float l2, t2, r2, b2;

		this->GetBoundingBox(l1, t1, r1, b1);
		obj->GetBoundingBox(l2, t2, r2, b2);

		if (CGame::IsOverlapping(l1, t1, r1, b1, l2, t2, r2, b2))
		{
			// Tạo sự kiện giả để xử lý
			LPCOLLISIONEVENT fakeEvent = new CCollisionEvent(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, obj, this);
			OnCollisionWith(fakeEvent);
			delete fakeEvent;
		}
	}
}

void CTailHitbox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_WIDTH / 2;
	t = y - TAIL_HEIGHT / 2	;
	r = l + TAIL_WIDTH;
	b = t + TAIL_HEIGHT;
}

void CTailHitbox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CBrickQues*>(e->obj))
		OnCollisionWithBrickQues(e);
	else if (dynamic_cast<CPlantEnemies*>(e->obj))
		OnCollisionWithPlantEnemies(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CBrickPSwitch*>(e->obj))
		OnCollisionWithBrickPSwitch(e);
}

void CTailHitbox::OnCollisionWithBrickPSwitch(LPCOLLISIONEVENT e)
{
	DebugOut(L"\t[TAIL ATTACK MODEL BRICK]\n\n");
	CBrickPSwitch* bricksp = dynamic_cast<CBrickPSwitch*>(e->obj);
	if (e->nx != 0)
	{

		if (bricksp->GetModel() == MODEL_PSWITCH)
		{
			DebugOut(L"\t[TAIL ATTACK MODEL PSWITCH]\n\n");
			bricksp->SetState(BRICK_STATE_NO_PSWITCH);
			bricksp->CreatePSwitch();
		}
		else if (bricksp->GetModel() == MODEL_COIN)
		{
			DebugOut(L"\t[TAIL ATTACK MODEL COIN]\n\n");
			bricksp->SetState(BRICK_STATE_BREAK);
		}

	}
}

void CTailHitbox::OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e)
{
	DebugOut(L"\t[TAIL ATTACK PLANTENEMIES]\n\n");
}

void CTailHitbox::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	DebugOut(L"\t[TAIL ATTACK KOOPA]\n\n");
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa->GetState() == KOOPA_STATE_DIE)
	{
		e->obj->Delete();
		return;
	}
	if (e->nx != 0) 
	{
		koopa->SetState(KOOPA_STATE_DEAD_UPSIDE);
		mario->AddScoreEffect(koopa->GetX(), koopa->GetY(), SCORE_100);
	}
}

void CTailHitbox::OnCollisionWithBrickQues(LPCOLLISIONEVENT e)
{
	DebugOut(L"\t[TAIL ATTACK BRICK QUES]\n\n");
	CBrickQues* questionBrick = dynamic_cast<CBrickQues*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!questionBrick || questionBrick->GetIsUnbox() || questionBrick->GetIsEmpty())
		return;
	
	if (e->nx != 0)
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		questionBrick->SetState(BRICK_QUES_STATE_UP);

		float xT, yT, minY;
		xT = questionBrick->GetX();
		yT = questionBrick->GetY();
		minY = questionBrick->GetMinY();

		if (questionBrick->GetItemType() == BRICK_QUES_COIN)
		{
			mario->SetCoin(mario->GetCoin() + 1);
			CCoin* coin = new CCoin(xT, yT);
			coin->SetState(COIN_SUM);
			scene->PushObject(coin);

			questionBrick->SetIsEmpty(true);
			mario->AddScoreEffect(coin->GetX(), coin->GetY(), SCORE_100);
		}
		else if (questionBrick->GetItemType() == BRICK_QUES_MUSHROOM_RED_OR_LEAF)
		{
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				CBaseMushroom* mushroom = new CBaseMushroom(xT, yT, MUSHROOM_TYPE_RED);
				//DebugOut(L"type mushroom  %d \n", mushroom->GetType());
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
		else if (questionBrick->GetItemType() == BRICK_QUES_MUSHROOM_GREEN)
		{
			CBaseMushroom* mushroom = new CBaseMushroom(xT, yT, MUSHROOM_TYPE_GREEN);
			mushroom->SetState(MUSHROOM_STATE_RISING);
			scene->PushObject(mushroom);
			questionBrick->SetIsEmpty(true);
		}
	}

}

void CTailHitbox::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	DebugOut(L"\t[TAIL ATTACK GOOMBA]\n\n");
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (e->nx != 0) 
	{
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		goomba->SetState(GOOMBA_STATE_DIE);
		mario->AddScoreEffect(goomba->GetX(), goomba->GetY(), SCORE_100);
	}
	
}
