﻿#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		    0.1f
#define MARIO_RUNNING_SPEED		    0.2f

#define MARIO_ACCEL_WALK_X	        0.0005f
#define MARIO_ACCEL_RUN_X		    0.0007f
#define MARIO_JUMP_SPEED_Y		    0.3f		
#define MARIO_JUMP_RUN_SPEED_Y	    0.35f		
#define MARIO_GRAVITY			    0.0005f		
#define MARIO_JUMP_DEFLECT_SPEED    0.15f		

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARRIO_STATE_GROWING	700
#define MARIO_CHANGE_TIME 500


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

// MARIO RACCOON
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 2000
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 2001

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 2100
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 2101

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 2200
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 2201

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2300
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 2301

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 2400
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 2401

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2500
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2501

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2600
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 2601

#pragma endregion

#define GROUND_Y 160.0f


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACCOON		3


#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_RACCOON_BBOX_WIDTH  20
#define MARIO_RACCOON_BBOX_HEIGHT 27
#define MARIO_RACCOON_FLY_BBOX_WIDTH  23
#define MARIO_RACCOON_FLY_BBOX_HEIGHT 26
#define MARIO_RACCOON_SITTING_BBOX_WIDTH  13
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 17

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)
#define MARIO_RACCOON_SIT_HEIGHT_ADJUST ((MARIO_RACCOON_BBOX_HEIGHT-MARIO_RACCOON_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_UNTOUCHABLE_TIME 2500

#define COLLISION_MARGIN 8.0f

#define SCORE_100 100
#define SCORE_1000 1000


#define SNAPOFFSET 1.5f    
#define TERMINAL_VELOCITY 0.2f



class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int targetLevel;
	int untouchable;
	ULONGLONG untouchable_start;
	ULONGLONG grow_start;//thời gian biến to
	ULONGLONG transform_start;// thời gian biến hình
	BOOLEAN isOnPlatform;
	int coin;
	int score;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickQues(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBall(LPCOLLISIONEVENT e);
	void OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickPSwitch(LPCOLLISIONEVENT e);
	void OnCollisionWithPSwitch(LPCOLLISIONEVENT e);
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

	bool isGrowing;// xác định mario đang biến hình từ nhỏ biến to
	bool isRaccoon;

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		grow_start = 0;
		transform_start = 0;
		isOnPlatform = false;
		coin = 0;
		score = 0;
		isGrowing = false;
		isRaccoon = false;

		targetLevel = -1;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void HandleSolidCollision(LPGAMEOBJECT gameobject, float objHeight);

	void SetLevel(int l);
	void SetCoin(int c) { this->coin = c; }
	void SetScore(int s) { score = s; }
	void SetY(int l) { this->y = l; }
	void SetX(int l) { this->x = l; }
	void SetLevelLower();
	void ResetVerticalMovement();

	int GetCoin() { return this->coin; }
	int GetScore() { return score; }
	bool GetIsOnPlatform() { return isOnPlatform; }
	float GetCurrentHeight() const;
	int GetLevel() { return this->level; }

	int IsGrowing() { return isGrowing ; }
	int IsRaccoon() { return isRaccoon; }
	int IsUntouchable() { return untouchable; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void AddScoreEffect(float xTemp, float yTemp, int scoreAdd);
	


};