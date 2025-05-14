#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Platform.h"
#include "Portal.h"
#include "BrickQues.h"
#include "BaseMushroom.h"

#include "Collision.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	vy += MARIO_GRAVITY * dt;
	vx += ax * dt;
	
	if (vy > TERMINAL_VELOCITY)
		vy = TERMINAL_VELOCITY;
	if (abs(vx) > abs(maxVx)) vx = maxVx;
	
	if (isGrowing )
	{
		if (GetTickCount64() - grow_start > MARIO_GROW_TIME)
		{
			if (level == MARIO_LEVEL_SMALL && targetLevel == MARIO_LEVEL_BIG)
				y -= 12; // Dịch trước khi gán level
			isGrowing = false;
			level = targetLevel;
			targetLevel = -1; // tránh bị lẫn 
		}
		else
		{
			//stop
			vx = 0;
			vy = 0;
		}
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (vy > 0.0f)
		isOnPlatform = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOut(L"out\n");
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CBrickQues*>(e->obj))
		OnCollisionWithBrickQues(e);
	else if (dynamic_cast<CBaseMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}
//======================================================================

float CMario::GetCurrentHeight() const
{
	if (level == MARIO_LEVEL_SMALL) {
		return MARIO_SMALL_BBOX_HEIGHT;
	}
	
	return (isSitting ? MARIO_BIG_SITTING_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT);
}

void CMario::ResetVerticalMovement()
{
	vy = 0;
	isOnPlatform = true;
}

void CMario::HandleSolidCollision(LPGAMEOBJECT gameobject)
{
	
	float collisionHeight = GetCurrentHeight();
	float distanceToPlatform = gameobject->GetY() - y;
	//DebugOut(L"collisionHeight :%f ,distanceToPlatform : %f \n", collisionHeight, distanceToPlatform);

	if (distanceToPlatform < collisionHeight + COLLISION_MARGIN) {
		float newY = gameobject->GetY() - collisionHeight;
		//DebugOut(L"newY :%f\n", newY);
		//DebugOut(L"collisionHeight + COLLISION_MARGIN :%f\n", collisionHeight + COLLISION_MARGIN);
		if (level == MARIO_LEVEL_SMALL) {
			newY -= SNAPOFFSET;
			//DebugOut(L"newY mario small:%f\n", newY);
		}
		else if (!isSitting) {
			newY += SNAPOFFSET;
			//DebugOut(L"newY mario big:%f\n", newY);
		}
		SetPosition(x, newY);
		ResetVerticalMovement();
	}
	
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					//DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithPlatForm(LPCOLLISIONEVENT e)
{
}

void CMario::OnCollisionWithBrickQues(LPCOLLISIONEVENT e)
{
	CBrickQues* questionBrick = dynamic_cast<CBrickQues*>(e->obj);

	if (!questionBrick || questionBrick->GetIsUnbox() || questionBrick->GetIsEmpty())
		return;
	if (e->ny < 0) {
		
		HandleSolidCollision(questionBrick);
		return;
	}

	if (e->ny > 0)
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		questionBrick->SetState(BRICK_QUES_STATE_UP);

		float xT, yT, minY;
		xT = questionBrick->GetX();
		yT = questionBrick->GetY();
		minY = questionBrick->GetMinY();

		if (questionBrick->GetItemType() == BRICK_QUES_COIN)
		{
			SetCoin(GetCoin() + 1);
			CCoin* coin = new CCoin(xT, yT);
			coin->SetState(COIN_SUM);
			scene->AddObject(coin);
		}
		else if (questionBrick->GetItemType() == BRICK_QUES_MUSHROOM_RED_OR_LEAF)
		{
			if (level == MARIO_LEVEL_SMALL) {
				CBaseMushroom* mushroom = new CBaseMushroom(xT, yT, MUSHROOM_TYPE_RED);
				DebugOut(L"type mushroom  %d \n", mushroom->GetType());
				mushroom->SetState(MUSHROOM_STATE_RISING);
				scene->AddObject(mushroom);
			}
			else if (level >= MARIO_LEVEL_BIG) {
				DebugOut(L"type mushroom  %d \n", 0);
			}
		}
		questionBrick->SetIsEmpty(true);
	}

}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CBaseMushroom* mushroom = dynamic_cast<CBaseMushroom*>(e->obj);
	

	if (mushroom->GetType() == MUSHROOM_TYPE_RED) 
	{
		if (!mushroom->IsDeleted()) {
			score += 1000;
			//AddScoreEffect(x, y - MARIO_BIG_BBOX_HEIGHT, 1000);
		}
		if (level == MARIO_LEVEL_SMALL)
		{
			isGrowing = true;
			grow_start = GetTickCount64();
			targetLevel = MARIO_LEVEL_BIG;  // vì sau khi nháy mario mới biến thành to
		}
	}

	mushroom->Delete();
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	if (isGrowing) 
	{
		if ((GetTickCount64() / 100) % 2 == 0)
			if (level == MARIO_LEVEL_BIG)
				animations->Get(GetAniIdBig())->Render(x, y);
			else animations->Get(GetAniIdSmall())->Render(x, y);
	}
	else 
	{
		animations->Get(aniId)->Render(x, y); // render bình thường
	}
	RenderBoundingBox();

	//DebugOutTitle(L"Coins: %d", coin);
	DebugOutTitle(L"Score: %d", score);
	
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}



void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::AddScoreEffect(float xTemp, float yTemp, int scoreAdd)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	if (scoreAdd == SCORE_100) {
		//CEffect* effect = new CEffect(xTemp, yTemp, EFFECT_SCORE_100);
		//scene->AddObject(effect);
	}
	score += scoreAdd;
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}



