#include "TailHitbox.h"
#include "debug.h"

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
CTailHitbox::CTailHitbox(float x, float y, CMario* mario)
{
	this->x = x;
	this->y = y;
	this->mario = mario;
	createTime = GetTickCount64();
}

void CTailHitbox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - createTime > TAIL_HITBOX_LIFE_TIME)
	{
		this->Delete(); 
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTailHitbox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_WIDTH / 2;
	t = y - TAIL_HEIGHT / 2	;
	r = x + TAIL_WIDTH;
	b = y + TAIL_HEIGHT;
}

void CTailHitbox::OnCollisionWith(LPCOLLISIONEVENT e)
{
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

}

void CTailHitbox::OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e)
{
}

void CTailHitbox::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
}

void CTailHitbox::OnCollisionWithBrickQues(LPCOLLISIONEVENT e)
{
	CBrickQues* questionBrick = dynamic_cast<CBrickQues*>(e->obj);

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
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	mario->AddScoreEffect(goomba->GetX(), goomba->GetY(), SCORE_100);
}
