#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_BRICK_DECOR 10001

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK 0
#define BRICK_DECOR	1


class CBrick : public CGameObject {
protected:
	int type;
public:
	CBrick(float x, float y, int t) : CGameObject(x, y) 
	{
		type = t;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};