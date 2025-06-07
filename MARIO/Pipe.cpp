#include "Pipe.h"
#include "PlayScene.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Mario.h"
#include "Collision.h"
#include "Textures.h"
#include "Game.h"

CPipe::CPipe(float x, float y, 
	float cell_width, float cell_height, int length, 
	int sprite_id_top, int sprite_id_middle, int sprite_id_end) : CGameObject(x, y) 
{
	this->length = length;
	this->cellWidth = cell_width;
	this->cellHeight = cell_height;
	this->spriteIdTop = sprite_id_top;
	this->spriteIdMiddle = sprite_id_middle;
	this->spriteIdEnd = sprite_id_end;

}

void CPipe::Render()
{
	if (!CheckObjectInCamera(this)) return;
	if (this->length <= 0) return;
	float yy = y;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdTop)->Draw(x, yy);
	yy += this->cellHeight;

	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(x, yy);
		yy += this->cellHeight;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(x, yy);

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	t = y - this->cellHeight / 2;
	l = x - this->cellWidth / 2;
	b = t + this->cellHeight * this->length;
	r = l + cellWidth;
}

void CPipe::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float drawX = l + rect.right / 2.0f;
	float drawY = t + rect.bottom / 2.0f;

	CGame::GetInstance()->Draw(drawX - cx, drawY - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

int CPipe::IsDirectionColliable(float nx, float ny)
{
	
	return 1;
}

int CPipe::IsBlocking()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsUsePipe() && PipeCanDown()) return 0;
	else if (mario->IsUsePipe() && PipeCanUp()) return 0;
	return (cellHeight > 10 && cellWidth > 16);
}
