#pragma once
#include "GameObject.h"



#define	MUSHROOM_WIDTH 10
#define MUSHROOM_BBOX_WIDTH 10
#define MUSHROOM_BBOX_HEIGHT 16



#define MUSHROOM_STATE_MOVING 100
#define MUSHROOM_STATE_IDLE 101
#define MUSHROOM_STATE_RISING 102

#define ID_ANI_RED_MUSHROOM 14000

#define MUSHROOM_TYPE_RED 1

#define MUSHROOM_GRAVITY 0.0005f
#define MUSHROOM_SPEED 0.06f
#define MUSHROOM_SPEED_RISING 0.03f


class CBaseMushroom : public CGameObject
{
protected:
	float ax;
	float ay;

	float startY;
	bool isPoppingUp = false;
	int type;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	

public:
	CBaseMushroom(float x, float y, int t);
	virtual void SetState(int state);

	void SetType(int t) { this->type = t; }
	int GetType() { return this->type; }

};

