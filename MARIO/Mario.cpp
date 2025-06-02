#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Platform.h"
#include "Portal.h"
#include "BrickQues.h"
#include "BrickPSwitch.h"
#include "BaseMushroom.h"
#include "Leaf.h"
#include "FireBall.h"
#include "PlantEnemies.h"

#include "Effect.h"
#include "Collision.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += MARIO_GRAVITY * dt;
	vx += ax * dt;
	//DebugOut(L"MARIO POSITION : %f , %f\n", x, y);
	//DebugOut(L"[MARIO ] IS HOLDING RUN KEY %d\n", isHoldingRunKey);
	if (vy > TERMINAL_VELOCITY)
		vy = TERMINAL_VELOCITY;
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	if (isGrowing)
	{
		if (GetTickCount64() - grow_start > MARIO_CHANGE_TIME)
		{
			if (level == MARIO_LEVEL_SMALL && targetLevel == MARIO_LEVEL_BIG)
				y -= 12; // Dịch trước khi gán level
			isGrowing = false;
			level = targetLevel;
			targetLevel = -1;
		}
		else
		{
			//stop
			vx = 0;
			vy = 0;
		}
	}

	if (isRaccoon)
	{
		if (GetTickCount64() - transform_start > MARIO_CHANGE_TIME)
		{
			if (level == MARIO_LEVEL_BIG)
				y -= 12; // Dịch trước khi gán level
			// xử lý hiệu ứng cho mario bùm biến thành raccoon 
			isRaccoon = false;
		}
		else
		{
			//stop
			vx = 0;
			vy = 0;
		}
	}
	
	if (isHoldingShell && holdingShell) {
		holdingShell->SetPosition(x + nx * 13.0f , y - 2.0f);
		holdingShell->SetSpeed(0.0f, 0.0f);
		if (!isHoldingRunKey)
		{
			isHoldingShell = false;
			holdingShell->SetHeld(false);
			float dir = nx >= 0 ? 1.0f : -1.0f;
			holdingShell->SetState(KOOPA_STATE_SHELL_MOVING);
			holdingShell->SetSpeed(dir * KOOPA_KICKED_SPEED, 0);
			holdingShell = nullptr;
		}
		if (holdingShell && holdingShell->IsComeback()) {
			isHoldingShell = false;
			holdingShell->SetHeld(false);
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
		if (e->ny < 0)
		{
			isOnPlatform = true;
		}
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
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CFireBall*>(e->obj))
		OnCollisionWithFireBall(e);
	else if (dynamic_cast<CPlantEnemies*>(e->obj))
		OnCollisionWithPlantEnemies(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBrickPSwitch*>(e->obj))
		OnCollisionWithBrickPSwitch(e);
	else if (dynamic_cast<CPSwitch*>(e->obj))
		OnCollisionWithPSwitch(e);
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
	vy = 0; ay = 0; 
	isOnPlatform = true;
}

void CMario::HandleSolidCollision(LPGAMEOBJECT gameobject, float objHeight)
{
	
	float marioHeight = GetCurrentHeight();
	float platformY = gameobject->GetY();
	float marioY = this->y;
	float marioBottom = marioY + marioHeight / 2;
	float platformTop = platformY - objHeight / 2;
	float overlap = marioBottom - platformTop ;
	if (overlap > -COLLISION_MARGIN && overlap < marioHeight) 
	{
		float newY = platformTop - marioHeight / 2;
		SetPosition(x, newY);
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

		if (e->ny < 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if(goomba->GetType() == GOOMBA_TYPE_WING && !goomba->IsAttack() )
					goomba->SetState(GOOMBAPARA_STATE_IS_ATTACK);
				else  goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				AddScoreEffect(goomba->GetX(),goomba->GetY(), SCORE_100);
			}
		}
		else // hit by Goomba
		{
			if (untouchable == 0)
			{
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					SetLevelLower();
				}
			}
		}
	
	// jump on top >> kill Goomba and deflect a bit 
	
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsVisible()) return;
	if (e->obj->GetState() == COIN_NOT_SUM || e->obj->GetState() == COIN_SWITCH_NOT_SUM)
		AddScoreEffect(e->obj->GetX(), e->obj->GetY(), SCORE_100);
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithBrickQues(LPCOLLISIONEVENT e)
{
	CBrickQues* questionBrick = dynamic_cast<CBrickQues*>(e->obj);

	if (!questionBrick || questionBrick->GetIsUnbox() || questionBrick->GetIsEmpty())
		return;
	if (e->ny < 0) {
		
		HandleSolidCollision(questionBrick, BRICK_QUES_BBOX_HEIGHT);
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
			scene->PushObject(coin);
			questionBrick->SetIsEmpty(true);
			AddScoreEffect(coin->GetX(),coin->GetY(), SCORE_100);
		}
		else if (questionBrick->GetItemType() == BRICK_QUES_MUSHROOM_RED_OR_LEAF)
		{
			if (level == MARIO_LEVEL_SMALL) 
			{
				CBaseMushroom* mushroom = new CBaseMushroom(xT, yT, MUSHROOM_TYPE_RED);
				//DebugOut(L"type mushroom  %d \n", mushroom->GetType());
				mushroom->SetState(MUSHROOM_STATE_RISING);
				scene->PushObject(mushroom);
				questionBrick->SetIsEmpty(true);
			}
			else if (level >= MARIO_LEVEL_BIG) 
			{
				CLeaf* leaf = new CLeaf(xT, yT);
				leaf->SetState(LEAF_STATE_RISE);
				scene->PushObject(leaf);
				questionBrick->SetIsEmpty(true);
			}
		}
		
	}

}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CBaseMushroom* mushroom = dynamic_cast<CBaseMushroom*>(e->obj);
	

	if (mushroom->GetType() == MUSHROOM_TYPE_RED) 
	{
		if (!mushroom->IsDeleted()) {
			//score += SCORE_1000;
			AddScoreEffect(x, y - MARIO_BIG_BBOX_HEIGHT, SCORE_1000);
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

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
		if (!leaf->IsDeleted()) {
			//score += SCORE_1000;
			AddScoreEffect(x, y - MARIO_BIG_BBOX_HEIGHT, SCORE_1000);
		}
		if (level == MARIO_LEVEL_RACCOON) {}
		else if (level == MARIO_LEVEL_BIG)
		{
			isRaccoon = true;
			transform_start = GetTickCount64();
			SetLevel(MARIO_LEVEL_RACCOON);
		}
		else {}


	leaf->Delete();
}

void CMario::OnCollisionWithFireBall(LPCOLLISIONEVENT e)
{
	if (untouchable) return;
	e->obj->Delete();
	if (state != MARIO_STATE_DIE)
	{
		SetLevelLower();
	}
	
}

void CMario::OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e)
{
	if (untouchable) return;
	if (state != MARIO_STATE_DIE)
	{
		SetLevelLower();
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa->GetState() == KOOPA_STATE_DIE)
	{
		e->obj->Delete();
		return;
	}
		
	if (e->ny < 0)
	{
		//DebugOut(L"[VA CHAM VOI KOOPA TU TREN XUONG]ISWING %d, state %d\n", koopa->IsWing(), koopa->GetState());
		if (!koopa->IsWing()) 
		{
			if (koopa->GetState() == KOOPA_STATE_WALKING)
			{
				//DebugOut(L"[MARIO]CHANGE KOOPA STATE WALKING -> SHELL STATE IDLE\n");
				//DebugOut(L"[MARIO]KOOPA X %f, Y %f\n", koopa->GetX(), koopa->GetY());
				AddScoreEffect(koopa->GetX(), koopa->GetY(), SCORE_100);
				koopa->SetState(KOOPA_STATE_SHELL);

			}
			else if (koopa->GetState() == KOOPA_STATE_SHELL_MOVING)
			{
				//DebugOut(L"[MARIO]CHANGE KOOPA SHELL STATE MOVING -> SHELL STATE IDLE\n");
				koopa->SetState(KOOPA_STATE_SHELL);
			}
			else if (koopa->GetState() == KOOPA_STATE_SHELL)
			{
				float direction = (x < koopa->GetX() + 4.0f) ? 1.0f : -1.0f;
				koopa->SetState(KOOPA_STATE_SHELL_MOVING);
				koopa->SetSpeed(direction * KOOPA_KICKED_SPEED, 0.0f);
			}
		}
		else
		{
			if (koopa->GetState() == KOOPA_STATE_JUMP)
			{
				DebugOut(L"[MARIO COLLISION GOOMBA WING ] LOSS WING\n");
				koopa->SetState(KOOPA_STATE_WALKING);
			}
			else
			{
				if (koopa->GetState() == KOOPA_STATE_WALKING)
				{
					//DebugOut(L"[MARIO]CHANGE KOOPA STATE WALKING -> SHELL STATE IDLE\n");
					//DebugOut(L"[MARIO]KOOPA X %f, Y %f\n", koopa->GetX(), koopa->GetY());
					AddScoreEffect(koopa->GetX(), koopa->GetY(), SCORE_100);
					koopa->SetState(KOOPA_STATE_SHELL);

				}
				else if (koopa->GetState() == KOOPA_STATE_SHELL_MOVING)
				{
					//DebugOut(L"[MARIO]CHANGE KOOPA SHELL STATE MOVING -> SHELL STATE IDLE\n");
					koopa->SetState(KOOPA_STATE_SHELL);
				}
				else if (koopa->GetState() == KOOPA_STATE_SHELL)
				{
					float direction = (x < koopa->GetX() + 4.0f) ? 1.0f : -1.0f;
					koopa->SetState(KOOPA_STATE_SHELL_MOVING);
					koopa->SetSpeed(direction * KOOPA_KICKED_SPEED, 0.0f);
				}
			}
		}
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else // hit by KOOPA
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() == KOOPA_STATE_SHELL)
			{
				if (isHoldingRunKey && !isHoldingShell) 
				{
					PickUpShell(koopa);
				}
				else 
				{
					float direction = (x < koopa->GetX()) ? 1.0f : -1.0f; // xác định hướng đá
					koopa->SetState(KOOPA_STATE_SHELL_MOVING);
					koopa->SetSpeed(direction * KOOPA_KICKED_SPEED, 0.0f);
				}
			}
			// (c) Nếu mai rùa đang chạy hoặc Koopa đang đi thì Mario bị mất level
			else if (koopa->GetState() == KOOPA_STATE_SHELL_MOVING || koopa->GetState() == KOOPA_STATE_WALKING || koopa->IsWing())
			{
				SetLevelLower();
			}
		}
	}
}

void CMario::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (!platform->IsBlocking() && platform->GetCellHeight() >= 9) 
	{
		if (e->ny < 0 && vy > 0 && platform->IsDirectionColliable(e->nx, e->ny))
		{
			//isOnPlatform = true;
			
			HandleSolidCollision(platform, platform->GetCellHeight());
			ResetVerticalMovement();
			
		}
	}
}

void CMario::OnCollisionWithBrickPSwitch(LPCOLLISIONEVENT e)
{
	CBrickPSwitch* bricksp = dynamic_cast<CBrickPSwitch*>(e->obj);
	//DebugOut(L"[BUTTON SWITCH MODEL :%d]\n", bricksp->GetModel());
	if (!bricksp || bricksp->IsEmpty()) 
	{
		return;
	}
		
	if (e->ny < 0) {
		HandleSolidCollision( bricksp , BRICK_MODEL_BBOX_HEIGHT);
		return;
	}
	if (e->ny > 0) 
	{
		
		if (bricksp->GetModel() == MODEL_PSWITCH) 
		{
			DebugOut(L"\t[STATE BRICK PSWITCH]\n\n");
			bricksp->SetState(BRICK_STATE_NO_PSWITCH);
			bricksp->CreatePSwitch();
		}
		else if (bricksp->GetModel() == MODEL_COIN) 
		{
			//DebugOut(L"\t[STATE COIN]\n\n");
			bricksp->SetState(BRICK_MODEL_STATE_UP);
			
		}
		
	}
}

void CMario::OnCollisionWithPSwitch(LPCOLLISIONEVENT e)
{
	CPSwitch* PS = dynamic_cast<CPSwitch*>(e->obj);
	if (!PS || PS->IsFinish() || PS->IsActivated())
	{
		HandleSolidCollision(PS, PSWITCH_ACTIVE_BBOX_HEIGHT);
		return;
	}

	if (e->ny < 0)
	{
		DebugOut(L"COLLISION WITH PSWITCH\n");
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		if (!PS->IsFinish()) 
		{
			PS->SetState(PSWITCH_STATE_ACTIVE);
		}
		
		HandleSolidCollision(PS, PSWITCH_ACTIVE_BBOX_HEIGHT);
		return;
	}
	
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

int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	if (!isOnPlatform) 
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
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
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
			}
	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
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
	else if (level == MARIO_LEVEL_RACCOON) 
		aniId = GetAniIdRaccoon();
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



	//RenderBoundingBox();

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
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else 
	{
		if (isSitting)
		{
			left = x - MARIO_RACCOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
}

void CMario::AddScoreEffect(float xTemp, float yTemp, int scoreAdd)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	
	if (scoreAdd == SCORE_100) {
		DebugOut(L"[create effect 100]\n");
		CEffect* effect = new CEffect(xTemp, yTemp, EFFECT_SCORE_100);
		scene->PushObject(effect);
	}
	else if (scoreAdd == SCORE_1000) 
	{
		CEffect* effect = new CEffect(xTemp, yTemp, EFFECT_SCORE_1000);
		scene->PushObject(effect);
	}
	score += scoreAdd;
}

void CMario::PickUpShell(CKoopa* shell)
{
	isHoldingShell = true;
	holdingShell = shell;
	shell->SetHeld(true);
	shell->SetState(KOOPA_STATE_HELD);
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	switch (this->level)
	{
	case MARIO_LEVEL_SMALL:
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT)/2;
		break;
	case MARIO_LEVEL_BIG:
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT)/2;
		break;
	case MARIO_LEVEL_RACCOON:
		y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT)/2;
		break;
	}

	level = l;
}

void CMario::SetLevelLower()
{
	if (level > MARIO_LEVEL_BIG)
	{
		SetLevel(MARIO_LEVEL_BIG);

		StartUntouchable();
	}
	else if (level > MARIO_LEVEL_SMALL)
	{
		SetLevel(MARIO_LEVEL_SMALL);

		StartUntouchable();
	}
	else
		SetState(MARIO_STATE_DIE);
}

