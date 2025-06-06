#pragma once
#include "GameObject.h"
#include "Mario.h"
#define TAIL_HEIGHT 8.0f
#define TAIL_WIDTH 8.0f
#define TAIL_HITBOX_LIFE_TIME 500

class CTailHitbox :public CGameObject
{
protected:
	CMario* mario;
	ULONGLONG createTime;
public:
	CTailHitbox(float x, float y, CMario* mario);
	void Render() override { RenderBoundingBox(); }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) ;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickPSwitch(LPCOLLISIONEVENT e);
	void OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickQues(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

};

