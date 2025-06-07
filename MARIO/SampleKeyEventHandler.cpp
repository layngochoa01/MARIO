#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	
	CMario* mario = (CMario *)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->SetJumpKeyHeld(true);
		if (mario->GetIsOnPlatform()) {
			mario->SetState(MARIO_STATE_JUMP);
		}
		else {
			if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
				if (mario->GetLevelRun() == LEVEL_RUN_MAX) mario->SetState(MARIO_STATE_FLY);
				else if (mario->CanFloat()) {
					//DebugOut(L"ROI VAO FLOAT \n");
					mario->SetState(MARIO_STATE_FLOAT);
				}
		}
		break;
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) mario->SetState(MARIO_STATE_TAIL_ATTACK);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_Z:
		if(mario->GetLevel() == MARIO_LEVEL_RACCOON)
			mario->SetState(MARIO_STATE_TAIL_ATTACK);
		break;
	case DIK_ESCAPE:
		CGame::GetInstance()->GetCurrentScene()->TogglePause();
		break;
	case DIK_UP:
		mario->SetPrepareUp(1);
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetJumpKeyHeld(false);

		if (mario->GetVY() < 0) {
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetHoldingRunKey(game->IsKeyDown(DIK_A));
		if (mario->IsHoldingRunKey())
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_S)) {
			if (mario->GetIsOnPlatform()) {
				mario->SetState(MARIO_STATE_JUMP);
			}
		}
		else if (game->IsKeyDown(DIK_DOWN)) {
			mario->SetState(MARIO_STATE_SIT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetHoldingRunKey(game->IsKeyDown(DIK_A));
		if (mario->IsHoldingRunKey())
		{
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			mario->SetHoldingRunKey(game->IsKeyDown(DIK_A));
		}
		else if (game->IsKeyDown(DIK_S)) {
			if (mario->GetIsOnPlatform()) {
				mario->SetState(MARIO_STATE_JUMP);
			}
		}
		else if (game->IsKeyDown(DIK_DOWN)) {
			mario->SetState(MARIO_STATE_SIT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}