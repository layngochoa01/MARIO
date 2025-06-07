#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Koopa.h"
#include "debug.h"

#define TIME_CLOCK_INIT		300
#define TIME_ONE_SECOND	1000

#define MARIO_FLY_FALL 0.0f
#define MARIO_FLYING 0.27f

#define SCORE_MAX 9999999
#define LEVEL_RUN_MAX 7
#define TIME_FLY 3000

#define MARIO_WALKING_SPEED		    0.1f
#define MARIO_RUNNING_SPEED		    0.17f

#define MARIO_ACCEL_WALK_X	        0.0003f
#define MARIO_ACCEL_RUN_X		    0.0005f
#define MARIO_JUMP_SPEED_Y		    0.24f		
#define MARIO_JUMP_RUN_SPEED_Y	    0.24f		
#define MARIO_GRAVITY			    0.0005f		
#define MARIO_JUMP_DEFLECT_SPEED    0.13f
#define SPEED_MARIO_WHEN_BLOCK 0.007f
#define SPEED_LEVEL_RUN 0.01f
#define MARIO_SPEED_USE_PIPE 0.02f


#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_KICK			350

#define MARIO_STATE_TAIL_ATTACK		360

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLY		800
#define MARIO_STATE_FLOAT		850
#define MARRIO_STATE_GROWING	700

#define MARIO_STATE_DOWNING_PIPE 900
#define MARIO_STATE_UPPING_PIPE 1000

#define MARIO_CHANGE_TIME 500
#define TIME_PREPARE_RUN 500
#define TAIL_ATTACK_TIME 400
#define FLOATING_TIME_MAX 500
#define MARIO_KICK 200
#define TIME_SPEED 150
#define TIME_WAIT_USE_PIPE 2000


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_NOT_JUMP_WALK_RIGHT 811
#define ID_ANI_MARIO_NOT_JUMP_WALK_LEFT 810

#define ID_ANI_MARIO_HOLDING_RIGHT 611
#define ID_ANI_MARIO_HOLDING_LEFT 610

#define ID_ANI_MARIO_HOLDING_JUMP_RIGHT 621
#define ID_ANI_MARIO_HOLDING_JUMP_LEFT 620

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_KICH_RIGHT 1003
#define ID_ANI_MARIO_KICH_LEFT 1002



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

#define ID_ANI_MARIO_SMALL_NOT_JUMP_WALK_RIGHT 1610
#define ID_ANI_MARIO_SMALL_NOT_JUMP_WALK_LEFT 1611

#define ID_ANI_MARIO_SMALL_HOLDING_RIGHT 1311
#define ID_ANI_MARIO_SMALL_HOLDING_LEFT 1310

#define ID_ANI_MARIO_SMALL_HOLDING_JUMP_RIGHT 1321
#define ID_ANI_MARIO_SMALL_HOLDING_JUMP_LEFT 1320

#define ID_ANI_MARIO_SMALL_KICH_RIGHT 1411
#define ID_ANI_MARIO_SMALL_KICH_LEFT 1410

// MARIO RACCOON
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 2000
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 2001

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 2100
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 2101

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 2010
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 2011

#define ID_ANI_MARIO_RACCOON_FLOATING_RIGHT 2020
#define ID_ANI_MARIO_RACCOON_FLOATING_LEFT 2021

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2300
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 2301

#define ID_ANI_MARIO_RACCOON_NOT_JUMP_WALK_RIGHT 2310
#define ID_ANI_MARIO_RACCOON_NOT_JUMP_WALK_LEFT 2311


#define ID_ANI_MARIO_RACCOON_HOLDING_RIGHT 2111
#define ID_ANI_MARIO_RACCOON_HOLDING_LEFT 2110

#define ID_ANI_MARIO_RACCOON_HOLDING_JUMP_RIGHT 2113
#define ID_ANI_MARIO_RACCOON_HOLDING_JUMP_LEFT 2112

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2500
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2501

#define ID_ANI_MARIO_RACCOON_KICH_RIGHT 2511
#define ID_ANI_MARIO_RACCOON_KICH_LEFT 2510

#define ID_ANI_MARIO_RACCOON_AROUND_FROM_RIGHT 2521
#define ID_ANI_MARIO_RACCOON_AROUND_FROM_LEFT 2520

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2600
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 2601

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 2620
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 2621

#define ID_ANI_MARIO_RACCOON_USE_PIPE 2650



#pragma endregion

#define GROUND_Y 160.0f


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACCOON		3


#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 26
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_RACCOON_BBOX_WIDTH  20
#define MARIO_RACCOON_BBOX_HEIGHT 26
#define MARIO_RACCOON_FLY_BBOX_WIDTH  23
#define MARIO_RACCOON_FLY_BBOX_HEIGHT 26
#define MARIO_RACCOON_SITTING_BBOX_WIDTH  21
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 14

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)
#define MARIO_RACCOON_SIT_HEIGHT_ADJUST ((MARIO_RACCOON_BBOX_HEIGHT-MARIO_RACCOON_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 14

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
	ULONGLONG kich_start;
	ULONGLONG speed_stop;
	ULONGLONG speed_start;
	ULONGLONG start_prepare;
	ULONGLONG tailAttackStart;
	ULONGLONG time_down_1_second;
	ULONGLONG time_floating;
	ULONGLONG time_wait_use_pipe;
	BOOLEAN isOnPlatform;
	bool isHoldingRunKey;
	bool checkUPCollisionX = false;
	bool isHoldingShell;
	bool isKich = false;
	bool isTailAttacking = false;
	bool isRunning;
	bool isFloating;
	bool isJumpKeyHeld;
	int levelRun;
	int coin;
	int score;
	int lives;
	int clock;
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
	void OnCollisionWithPipe(LPCOLLISIONEVENT e);
	void OnCollisionWithCard(LPCOLLISIONEVENT e);
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

	float startUsePiPeY;   // Tọa độ Y bắt đầu dùng pipe

	bool isDown;        
	bool isUp;         
	bool isUsePipe;        // Mario đang dùng ống
	bool isPrepareUp;

	bool isGrowing;// xác định mario đang biến hình từ nhỏ biến to
	bool isTransRaccoon;// xác định mario đang biến hình từ to thành chồn
	bool isFlying = false ;
	CKoopa* holdingShell = nullptr;

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		kich_start = 0;
		tailAttackStart = 0;
		untouchable = 0;
		untouchable_start = -1;
		grow_start = 0;
		transform_start = 0;

		level = MARIO_LEVEL_SMALL;
		isOnPlatform = false;
		coin = 0;
		score = 0;
		isGrowing = false;
		isTransRaccoon = false;
		isHoldingRunKey = false;
		isHoldingShell = false;
		isJumpKeyHeld = false;
		isFloating = false;
		isDown = false;
		isUp = false;
		isUsePipe = false;
		isPrepareUp = false;
		targetLevel = -1;
		lives = 4;
		clock = TIME_CLOCK_INIT;
		time_down_1_second = GetTickCount64();	
		isRunning = false;
		levelRun = 0;
		speed_start = 0;
		speed_stop = 0;
		start_prepare = 0;
		time_floating = 0;
		time_wait_use_pipe = 0;
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
	void SetIsRunning(bool b) { isRunning = b; }
	void SetPrepareUp(int x) { isPrepareUp = x; };
	void ResetVerticalMovement();
	void DownTimeClock1Second();
	void ClockReset() { this->clock = TIME_CLOCK_INIT; };

	int IsUsePipe() { return isUsePipe; }
	int IsPrepareUp() { return isPrepareUp; }
	int GetCoin() { return this->coin; }
	int GetScore() { return score; }
	int GetLevelRun() { return levelRun; }
	int GetClock() { return clock; }
	bool GetIsFlying() { return isFlying; }
	bool GetJumpKeyHeld() { return isJumpKeyHeld; }
	bool CanFloat();
	bool GetIsOnPlatform() { return isOnPlatform; }
	float GetCurrentHeight() const;
	int GetLevel() { return this->level; }

	int IsGrowing() { return isGrowing; }
	bool IsTransRaccoon() { return isTransRaccoon; }
	bool IsUntouchable() { return untouchable; }
	bool IsFloating() { return isFloating; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void AddScoreEffect(float xTemp, float yTemp, int scoreAdd);
	bool GetIsRunning() { return isRunning; }
	bool IsHoldingRunKey() { return isHoldingRunKey; }
	void SetHoldingRunKey(int s) { isHoldingRunKey = s; }
	void SetJumpKeyHeld(bool value) { isJumpKeyHeld = value; }
	void SetTransRaccoon(int s) { isTransRaccoon = s; }
	void PickUpShell(CKoopa* shell);
	void AddLife(int amount) { lives += amount; }
	void LoseLife();
	int GetLives() { return lives; }

};