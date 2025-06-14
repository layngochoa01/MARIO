﻿#include "HUD.h"
#include "PlayScene.h"
#include "Mario.h"
CHUD::CHUD(float x, float y) 
{
	this->x = x;
	this->y = y;
}

void CHUD::DrawNumber(int n, float xTemp, float yTemp)
{
	if (n == 0) CAnimations::GetInstance()->Get(ID_ANI_0)->Render(xTemp, yTemp);
	else if (n == 1) CAnimations::GetInstance()->Get(ID_ANI_1)->Render(xTemp, yTemp);
	else if (n == 2) CAnimations::GetInstance()->Get(ID_ANI_2)->Render(xTemp, yTemp);
	else if (n == 3) CAnimations::GetInstance()->Get(ID_ANI_3)->Render(xTemp, yTemp);
	else if (n == 4) CAnimations::GetInstance()->Get(ID_ANI_4)->Render(xTemp, yTemp);
	else if (n == 5) CAnimations::GetInstance()->Get(ID_ANI_5)->Render(xTemp, yTemp);
	else if (n == 6) CAnimations::GetInstance()->Get(ID_ANI_6)->Render(xTemp, yTemp);
	else if (n == 7) CAnimations::GetInstance()->Get(ID_ANI_7)->Render(xTemp, yTemp);
	else if (n == 8) CAnimations::GetInstance()->Get(ID_ANI_8)->Render(xTemp, yTemp);
	else  CAnimations::GetInstance()->Get(ID_ANI_9)->Render(xTemp, yTemp);
}

void CHUD::DrawCard(int n, float xTemp, float yTemp)
{
}

void CHUD::Render()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	// Vẽ khung đen + nền HUD

	RenderBlackBackground();
	RenderHUD(x + ADJUST_HUD_X, y + ADJUST_HUD_Y, ROWS_HUD, COLS_HUD);
	for (int i = 0; i < 3; i++)
	{
		RenderHUD(x + ADJUST_HUD_CARD_X + DISTANCE_CARD * i, y + ADJUST_HUD_Y, ROWS_HUD, COLS_HUD_CARD);
	}
	// chữ M
	RenderSym(ID_ANI_M_SYMBOL, x, y, POSITION_M_SYMBOL_X, POSITION_M_SYMBOL_Y);
	//WORLD 1
	RenderSym(ID_ANI_WORLD_SYMBOL, x, y, POSITION_M_SYMBOL_X + 8.0f, POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
	DrawNumber(1, x + POSITION_M_SYMBOL_X + 30.0f, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
	//x
	RenderSym(ID_ANI_X_SYMBOL, x, y, POSITION_M_SYMBOL_X + OFFSET_X_SYMBOL_FROM_M, POSITION_M_SYMBOL_Y);
	//clock
	RenderSym(ID_ANI_CLOCK_SYMBOL, x, y, POSITION_M_SYMBOL_X + 128.0f, POSITION_M_SYMBOL_Y);
	//coin
	RenderSym(ID_ANI_COIN_SYMBOL, x, y, POSITION_M_SYMBOL_X + 138.0f, POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);


	if (!mario) return;
	else 
	{
		//WRITE SCORE
		int score = mario->GetScore();
		DrawNumber(score / 1000000, x + POSITION_M_SYMBOL_X + 50.0f, y + POSITION_M_SYMBOL_Y);
		DrawNumber((score / 100000) % 10, x + POSITION_M_SYMBOL_X + 50.0f + 8.0f, y + POSITION_M_SYMBOL_Y);
		DrawNumber((score / 10000) % 10, x + POSITION_M_SYMBOL_X + 50.0f + 8.0f * 2, y + POSITION_M_SYMBOL_Y);
		DrawNumber((score / 1000) % 10, x + POSITION_M_SYMBOL_X + 50.0f + 8.0f * 3, y + POSITION_M_SYMBOL_Y);
		DrawNumber((score / 100) % 10, x + POSITION_M_SYMBOL_X + 50.0f + 8.0f * 4, y + POSITION_M_SYMBOL_Y);
		DrawNumber((score / 10) % 10, x + POSITION_M_SYMBOL_X + 50.0f + 8.0f * 5, y + POSITION_M_SYMBOL_Y);
		DrawNumber(score % 10, x + POSITION_M_SYMBOL_X + 50.0f + 8.0f * 6, y + POSITION_M_SYMBOL_Y);
		
		// WRITE UP
		int live = mario->GetLives();
		DrawNumber(live % 10, x + POSITION_M_SYMBOL_X + 20.0f, y + POSITION_M_SYMBOL_Y);
		//WRITE COIN
		int coin = mario->GetCoin();
		if (coin / 10 > 0) {
			DrawNumber(coin / 10, x + POSITION_M_SYMBOL_X + 140.0f + 7.0f, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
		DrawNumber(coin % 10, x + POSITION_M_SYMBOL_X + 140.0f + 15.0f, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		//WRITE CLOCK
		int clock = mario->GetClock();
		DrawNumber(clock / 100, x + POSITION_M_SYMBOL_X + 130.0f + 8.0f, y + POSITION_M_SYMBOL_Y);
		DrawNumber((clock / 10) % 10, x + POSITION_M_SYMBOL_X + 130.0f + 8.0f * 2, y + POSITION_M_SYMBOL_Y);
		DrawNumber(clock % 10, x + POSITION_M_SYMBOL_X + 130.0f + 8.0f * 3, y + POSITION_M_SYMBOL_Y);
	
		for (int i = 0; i < 6; i++) 
		{
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_OFF)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN * i, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}

		CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_MAX_OFF)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN * 7, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		
		if (mario->GetLevelRun() > 1)
		{
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_ON)->Render(x + ADJUST_X_POWER_POSITION, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
		if (mario->GetLevelRun() > 2) {
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_ON)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
		if (mario->GetLevelRun() > 3) {
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_ON)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN * 2, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
		if (mario->GetLevelRun() > 4) {
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_ON)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN * 3, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
		if (mario->GetLevelRun() > 5) {
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_ON)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN * 4, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
		if (mario->GetLevelRun() > 6) {
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_ON)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN * 5, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
		if (mario->GetLevelRun() == LEVEL_RUN_MAX) {
			CAnimations::GetInstance()->Get(ID_ANI_POWER_HUD_MAX_ON)->Render(x + ADJUST_X_POWER_POSITION + DISTANCE_EVERY_LEVEL_RUN * 7, y + POSITION_M_SYMBOL_Y - OFFSET_WORLD_SYMBOL_FROM_M);
		}
	}
}

void CHUD::RenderBlackBackground()
{
	for (int i = 0; i < ROWS; i++) 
	{
		for (int j = 0; j < COLS; j++) 
		{
			CAnimations::GetInstance()->Get(ID_ANI_BLACK)->Render(x + j * CELL_WIDTH_BLACK_RECTANGEL, y + i * CELL_HEIGHT_BLACK_RECTANGEL);
		}
	}
}

void CHUD::RenderHUD(float tx, float ty, int r, int c)
{
	int aniId;
	for (int i = 0; i < r; i++) 
	{
		for (int j = 0; j < c; j++) 
		{
			if (i == 0) 
			{
				if(j == 0) aniId = ID_ANI_HUD_TOP_L;
				else if(j == c - 1) aniId = ID_ANI_HUD_TOP_R;
				else aniId = ID_ANI_HUD_TOP_M; 
			}
			else if (i == r - 1) 
			{
				if (j == 0) aniId = ID_ANI_HUD_BOT_L;
				else if (j == c - 1) aniId = ID_ANI_HUD_BOT_R;
				else aniId = ID_ANI_HUD_BOT_M;
			}
			else 
			{
				if (j == 0) aniId = ID_ANI_HUD_MID_L; 
				else if (j == c - 1) aniId = ID_ANI_HUD_MID_R; 
				else aniId = ID_ANI_HUD_MID_M; 
			}
			CAnimations::GetInstance()->Get(aniId)->Render(tx + j * CELL_WIDTH , ty + i * CELL_HEIGHT);
		}
	}
}

void CHUD::RenderSym(int id, float tx, float ty, float offsetX, float offsetY)
{
	CAnimations::GetInstance()->Get(id)->Render(tx + offsetX, ty + offsetY);
}

