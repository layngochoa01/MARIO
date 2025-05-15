#pragma once
#include"GameObject.h"

#define ID_ANI_LEAF_LEFT 15000
#define ID_ANI_LEAF_RIGHT 15001

#define	LEAF_WIDTH 16
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 13

#define LEAF_SPEEP_RISING 0.2f
#define LEAF_SPEED_FALL 0.000015f
#define LEAF_SPEED_SWAY 0.03f
#define LEAF_TIME_SWAY 500

#define LEAF_STATE_RISE 100
#define LEAF_STATE_FALL 101
#define LEAF_STATE_IDLE 102


#define LEAF_GRAVITY 0.0003f

class CLeaf : public CGameObject
{
protected:
	float ax, ay;
	bool isCollected;

	ULONGLONG sway_start;
	int direction;
	bool isOnPlatform;

public:
	CLeaf(float x, float y);

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 0; }
	bool IsOnPlatform() { return isOnPlatform; }
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);

	void SetState(int s);
};

