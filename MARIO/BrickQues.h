#pragma once
#include "GameObject.h" 

#define BRICK_QUES_BBOX_HEIGHT 15.0f
#define BRICK_QUES_BBOX_WIDTH 15.0f
// BRICK MOVE 
#define BRICK_QUES_SPEED_UP 0.04f
#define BRICK_QUES_SPEED_DOWN 0.04f

#define JUMP_OFFSET 14.0f

//STATE BRICK QUESTION

#define BRICK_QUES_STATE_UP 101

// Animation id
#define ID_ANI_BRICK_QUES 13000
#define ID_ANI_BRICK_EMPTY 13001

// TYPE item BRICK
#define BRICK_QUES_COIN 1
#define BRICK_QUES_MUSHROOM_RED_OR_LEAF 2
#define BRICK_QUES_MUSHROOM_GREEN 3
#define BRICK_QUES_LEAF 4
#define BRICK_QUES_MUSHROOM_RED 5



class CBrickQues : public CGameObject
{
protected:
	float ay, minY;
	float startX, startY;// vi tri ban dau cua brick

	bool isUnbox ;// brick up, con dau cham hoi
	bool isEmpty ;
	int itemType;
	
	int IsCollidable() { return 1; }
	int IsBlocking() { return 1; }
public:
	CBrickQues(float x, float y, int item) : CGameObject(x, y)
	{
		this->ay = 0;
		this->minY = y - BRICK_QUES_BBOX_HEIGHT + JUMP_OFFSET;
		this->startX = x;
		this->startY = y;
		this->itemType = item;
		isUnbox = false;
		isEmpty = false;
	}
	
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);

	float GetMinY() { return minY; }
	BOOLEAN GetIsUnbox() { return isUnbox; }
	BOOLEAN GetIsEmpty() { return isEmpty; }
	int GetItemType() { return itemType; }
	float GetX() { return CGameObject::GetX(); }
	float GetY() { return CGameObject::GetY(); }

	void SetIsUnbox(BOOLEAN b) { isUnbox = b; }
	void SetIsEmpty(BOOLEAN b) { isEmpty = b; }
	void SetItemType(int i_t) { this->itemType = i_t; }
	void SetState(int state);



};

