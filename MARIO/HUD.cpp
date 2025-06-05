#include "HUD.h"
#include "PlayScene.h"
#include "Mario.h"
CHUD::CHUD(float x, float y) 
{
	this->x = x;
	this->y = y;
}

void CHUD::DrawNumber(int n, float xTemp, float yTemp)
{
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

	RenderHUD(x + ADJUST_HUD_CARD_X, y + ADJUST_HUD_Y, ROWS_HUD, COLS_HUD_CARD);
	RenderHUD(x + ADJUST_HUD_CARD_X + DISTANCE_CARD * 1, y + ADJUST_HUD_Y, ROWS_HUD, COLS_HUD_CARD);
	RenderHUD(x + ADJUST_HUD_CARD_X + DISTANCE_CARD * 2, y + ADJUST_HUD_Y, ROWS_HUD, COLS_HUD_CARD);
	// WORLD
	//DrawNumber(1, x + POSITION_WORLD_X, y - ADJUST_Y_POWER_POSITION);

	// Biểu tượng M
	//CAnimations::GetInstance()->Get(ID_ANI_M_SYMBOL)->Render(x + POSITION_M_SYMBOL_X, y - ADJUST_Y_POWER_POSITION_UNDER - 1);

	if (!mario) return;
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

