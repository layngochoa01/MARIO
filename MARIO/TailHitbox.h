#pragma once
#include "GameObject.h"
#include "Mario.h"
#define TAIL_HEIGHT 12.0f
#define TAIL_WIDTH 14.0f
#define TAIL_HITBOX_LIFE_TIME 700

class CTailHitbox :public CGameObject
{
protected:
	ULONGLONG createTime;
	int IsCollidable() { return 1;  }
	int IsBlocking() { return 0; }
public:
	CTailHitbox(float x, float y);
	void Render() { RenderBoundingBox(); }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) ;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickPSwitch(LPCOLLISIONEVENT e);
	void OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickQues(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

};

