#pragma once
#include "GameObject.h"
#include "PSwitch.h"
#include "Coin.h"


#define ID_ANI_BRICK_MODEL 10002
#define ID_ANI_BRICK_EMPTY 10000

#define BRICK_MODEL_BBOX_WIDTH 16
#define BRICK_MODEL_BBOX_HEIGHT 15

#define BRICK_MODEL_SPEED_UP 0.05f
#define BRICK_MODEL_SPEED_DOWN 0.04f
#define JUMP_OFFSET 4

#define BRICK_STATE_HAS_MODEL 100
#define BRICK_STATE_NO_PSWITCH 101
#define BRICK_STATE_COIN 102
#define BRICK_STATE_BREAK 103
#define BRICK_MODEL_STATE_UP 104

#define MODEL_COIN 0
#define MODEL_PSWITCH 1


class CBrickPSwitch : public CGameObject
{
protected:
	float ay, minY;
	float startX, startY;

	CPSwitch* PS;
	CCoin* coin;

	bool isEmpty;
	bool isBroken;
	int state;
	int model;
	bool isTransformedToCoin;         // Đang ở trạng thái coin?
	bool isSwitch;                    // Có phải là P-Switch?
	bool isCoin;
	bool isStateUpDown = false;
	int IsCollidable() { return 1; }
	int IsBlocking() { return 1; }
public:
	CBrickPSwitch(float x, float y, int m) : CGameObject(x, y) 
	{
		this->startX = x;
		this->startY = y;

		model = m;
		isEmpty = false;
		state = BRICK_STATE_HAS_MODEL;
		isBroken = false;
		if (m == MODEL_COIN) 
		{
			this->ay = 0;
			this->minY = y - BRICK_MODEL_BBOX_HEIGHT + JUMP_OFFSET;
			isTransformedToCoin = false;
			isSwitch = false;
			isCoin = true;
		}
		else 
		{
			this->ay = 0;
			this->minY = 0;
			isTransformedToCoin = false;
			isSwitch = true;
			isCoin = false;
		}
		PS = nullptr;
		coin = nullptr;
	};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);
	bool IsEmpty() const { return isEmpty; }
	void SetState(int s);
	
	int IsBroken() { return this->isBroken; }
	bool IsCoin() { return this->isCoin; }
	int GetModel() { return this->model; }
	void CreatePSwitch();

	void SetPSwitch(CPSwitch* ps) { this->PS = ps; }
	CPSwitch* GetPSwitch() { return PS; }
};

