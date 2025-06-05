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

#define POSITION_M_SYMBOL_X 18.0f
#define POSITION_M_SYMBOL_Y 22.0f

#define OFFSET_WORLD_SYMBOL_FROM_M 10.0f
#define OFFSET_X_SYMBOL_FROM_M 13.0f

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

#define ID_ANI_0	17540
#define ID_ANI_1	17541
#define ID_ANI_2	17542
#define ID_ANI_3	17543
#define ID_ANI_4	17544
#define ID_ANI_5	17545
#define ID_ANI_6	17546
#define ID_ANI_7	17547
#define ID_ANI_8	17548
#define ID_ANI_9	17549

#define ID_ANI_M_SYMBOL 17550
#define ID_ANI_COIN_SYMBOL 17552
#define ID_ANI_WORLD_SYMBOL 17551
#define ID_ANI_CLOCK_SYMBOL 17553
#define ID_ANI_X_SYMBOL 17554
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
	void RenderSym(int id, float tx, float ty, float offsetX, float offsetY);
	//virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
};

