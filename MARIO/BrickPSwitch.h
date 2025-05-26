#pragma once
#include "GameObject.h"
#include "PSwitch.h"


#define ID_ANI_BRICK_P_SWITCH 10002
#define ID_ANI_BRICK_EMPTY 10000

#define BRICK_PSWITCH_BBOX_WIDTH 16
#define BRICK_PSWITCH_BBOX_HEIGHT 15

#define BRICK_STATE_HAS_PSWITCH 0
#define BRICK_STATE_NO_PSWITCH 1


class CBrickPSwitch : public CGameObject
{
protected:
	bool isEmpty;
	int state;
	
	int IsCollidable() { return 1; }
	int IsBlocking() { return 1; }
public:
	CBrickPSwitch(float x, float y) : CGameObject(x, y) 
	{
		isEmpty = false;
		state = BRICK_STATE_HAS_PSWITCH;
	};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);
	bool IsEmpty() const { return isEmpty; }
	void SetState(int s);
};

