#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_SPEEP_UP 0.2f
#define COIN_SPEED_FALL 0.1f

#define COIN_NOT_SUM 200
#define COIN_SUM 100

class CCoin : public CGameObject {
protected:
	float ay;
	bool canCollect;
public:
	CCoin(float x, float y) : CGameObject(x, y) 
	{
		this->state = COIN_NOT_SUM;
		this->ay = 0.0005f;
		this->canCollect = false;

	};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }

	void OnNoCollision(DWORD dt);

	void SetState(int s);
	void SetCanCollect(bool b) { canCollect = b; }

	bool GetCanCollect() { return canCollect; }
	bool GetItemType() { return 1; }

};