#pragma once
#include "GameObject.h"

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_MOVING_SPEED 0.05f


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 14

#define MUSHROOM_STATE_IDLE	100
#define MUSHROOM_STATE_MOVING 101
#define MUSHROOM_STATE_COLLECTED 102

#define ID_ANI_RED_MUSHROOM 14000

#define MUSHROOM_TYPE_RED 1


class CBaseMushroom : public CGameObject
{
protected:
	float ax;
	float ay;

	int type;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e);

public:
	CBaseMushroom(float x, float y, int t);
	virtual void SetState(int state);

	void SetType(int t) { this->type = t; }
	int GetType() { return this->type; }

};

