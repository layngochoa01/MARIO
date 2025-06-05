#pragma once
#include "GameObject.h"
#include "Sprite.h"

#define ADJUST_HUD_X_POSITION 0.0f
#define ADJUST_HUD_Y_POSITION	188.0f
#define CELL_WIDTH_BLACK_RECTANGEL 8.0f
#define CELL_HEIGHT_BLACK_RECTANGEL 8.0f
#define ROWS 7
#define COLS 45

#define ADJUST_HUD_X	8.0f
#define ADJUST_HUD_Y	5.0f
#define CELL_WIDTH 8.0f
#define CELL_HEIGHT 8.0f
#define ROWS_HUD 4
#define COLS_HUD 22

#define ADJUST_HUD_CARD_X 205.0f
#define DISTANCE_CARD	26.0f
#define COLS_HUD_CARD 3

#pragma region HUB
#define ID_ANI_BLACK 17500
#define ID_ANI_NULL 17501

#define ID_ANI_HUD_TOP_L	17510
#define ID_ANI_HUD_TOP_M	17511
#define ID_ANI_HUD_TOP_R	17512

#define ID_ANI_HUD_MID_L	17520
#define ID_ANI_HUD_MID_M	17521
#define ID_ANI_HUD_MID_R	17522

#define ID_ANI_HUD_BOT_L	17530
#define ID_ANI_HUD_BOT_M	17531
#define ID_ANI_HUD_BOT_R	17532

#pragma endregion 

class CHUD 
{
protected:
	float x, y;
public:
	CHUD(float x, float y);
	void DrawNumber(int n, float xTemp, float yTemp);
	void DrawCard(int n, float xTemp, float yTemp);   
	virtual void Render();
	void RenderBlackBackground();
	void RenderHUD(float tx, float ty, int r, int c);

	//virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
};

