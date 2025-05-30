﻿#pragma once
#include "GameObject.h"

#define GOOMBA_TYPE_BASE 0
#define GOOMBA_TYPE_WING 1

#define GOOMBA_GRAVITY 0.005f
#define GOOMBA_WALKING_SPEED 0.05f



#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500
#define TIME_WALKING 2000
#define TIME_JUMP_SMALL 650

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 101
#define GOOMBA_STATE_UPSIDE 102
#define GOOMBAPARA_STATE_FLY 103
#define GOOMBAPARA_STATE_IS_ATTACK 104

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMNA_UPSIDE 5002

#define ID_ANI_GOOMBAPARA_WALKING 5006
#define ID_ANI_GOOMBAPARA_DIE 5007
#define ID_ANI_GOOMBAPARA_UPSIDE 5008

#define ID_ANI_GOOMBAPARA_FLY_WALKING 5003
#define ID_ANI_GOOMBAPARA_FLY_JUMP 5004
#define ID_ANI_GOOMBAPARA_FLY_UPSET 5005



class CGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	int type;
	int jumpCount;

	bool isUpside;// GOOMBA BỊ LẬT 
	bool isAttack; // GOOMBAPARA bị đạp mất cánh
	bool isOnGround;
	bool isJump;
	ULONGLONG die_start;
	ULONGLONG walking_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking()
	{
		if (type == GOOMBA_TYPE_BASE) return 1;
		return 0;
	}
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);

	int GetAniGoombaBase();
	int GetAniGoombaPara();

public:
	bool IsOnGround() { return isOnGround; }
	bool IsAttack() { return isAttack; }
	int GetType() { return this->type; }

	CGoomba(float x, float y, int t);
	virtual void SetState(int state);
};