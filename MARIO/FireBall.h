#pragma once
#include "GameObject.h"

#define FIREBALL_BBOX_WIDTH 9
#define FIREBALL_BBOX_HEIGH 8

#define FIREBALL_SPEED 0.05f
#define FIREBALL_GRAVITY 0.0f

#define ID_ANI_FIREBALL_LEFT 12100
#define ID_ANI_FIREBALL_RIGHT 12101


class CFireBall : public CGameObject
{
protected:
	float ax, ay;
	int direction;
	virtual int IsCollidable() { return 0; }
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CFireBall(float x, float y) : CGameObject(x, y) 
	{
		this->ax = 0;
		this->ay = FIREBALL_GRAVITY;
		this->vx = 0;
		this->vy = 0;
		this->direction = 1;
	}
	CFireBall(float bx, float by, float targetX, float targetY);
	void OnNoCollision(DWORD dt);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() ;
};

