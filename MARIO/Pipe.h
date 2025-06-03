#pragma once
#include"GameObject.h"
#define ID_ANI_PIPE 12000
#define PIPE_BBOX_WIDTH 30
#define PIPE_BBOX_HEIGHT 47
class CPipe : public CGameObject
{
protected:
	int length;			
	float cellWidth;
	float cellHeight;
	int spriteIdTop, spriteIdMiddle, spriteIdEnd;
public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_top, int sprite_id_middle, int sprite_id_end) ;
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int GetLength() { return length; }
	float GetCellWidth() { return cellWidth; };
	float GetCellHeight() { return cellHeight; };
	int IsDirectionColliable(float nx, float ny);
	virtual int IsBlocking() { return 1; };

};

