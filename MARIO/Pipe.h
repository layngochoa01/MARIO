#pragma once
#include"GameObject.h"
#define ID_ANI_PIPE 12000
#define PIPE_BBOX_WIDTH 30
#define PIPE_BBOX_HEIGHT 47
class CPipe : public CGameObject
{
public:
	CPipe(float x, float y) : CGameObject(x, y) {};
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

};

