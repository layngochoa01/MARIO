#pragma once
#include "GameObject.h"

#define PSWITCH_GRAVITY 0.0005f
#define PSWITCH_SPEED_RISING 0.03f

#define PSWITCH_BBOX_WIDTH 15
#define PSWITCH_NO_ACTIVE_BBOX_HEIGHT 15
#define PSWITCH_ACTIVE_BBOX_HEIGHT 6

#define TIME_LIVE_PSWITCH_ACTIVE 2000

#define PSWITCH_STATE_IDLE 1
#define PSWITCH_STATE_RISING 2
#define PSWITCH_STATE_ACTIVE 3

#define ANI_ID_PSWITCH_NOT_ACTIVE	12301
#define ANI_ID_PSWITCH_ACTIVE	12300
class CPSwitch : public CGameObject
{
protected:
	float  ay;
	float vy;
	float startY;
	bool isFinish; // het time bien doi thanh coin
	bool activated;// mario dap nut
	bool visible; // 1: bi marrio dam, dang troi len
	ULONGLONG time_live; // thời gian tro lai cua COIN


	void SetVisible(bool v) { visible = v; }
	bool IsVisible() const { return visible; }
	
	

public:
	CPSwitch(float x, float y) : CGameObject(x, y) 
	{
		ay = PSWITCH_GRAVITY;
		vy = -PSWITCH_SPEED_RISING;
		startY = y;
		visible = true;
		activated = false;
		isFinish = false;
		time_live = 0;
		SetState(PSWITCH_STATE_RISING);
	};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int s);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void OnNoCollision(DWORD dt);
	bool IsActivated() const { return activated; }
	bool IsFinish() { return this->isFinish; }
	void SetFinish(int s) { isFinish = s; }
};

