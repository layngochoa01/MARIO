#pragma once
#include "GameObject.h"
#include "Platform.h"
#include "BrickPSwitch.h"
#define KOOPA_GRAVITY         0.001f    
#define KOOPA_WALK_SPEED      0.025f     // Tốc độ di chuyển
#define KOOPA_KICKED_SPEED    0.2f     // Tốc độ khi bị đá
#define KOOPA_JUMP_DEFLECT    0.2f      // Lực nảy khi bị giết
#define KOOPA_JUMP_DEATH 0.3f

#define KOOPA_GRAVITY_WING 0.0004f
#define KOOPA_JUMP_SPEED 0.17f

#define KOOPA_DEFEND_TIMEOUT  10000      // Thời gian ở trong mai (ms)
#define KOOPA_COMEBACK_TIME   8000      // Thời gian trước khi hồi phục
#define KOOPA_TIME_DELAY_AROUND 400
#define KOOPA_JUMP_RELEASE 1000
#define KOOPA_DIE_TIME 1500

#define KOOPA_TYPE_RED  1
#define KOOPA_TYPE_GREEN  2
#define KOOPA_TYPE_GREEN_WING  3

#define ID_ANI_RED_WALK_RIGHT   16000
#define ID_ANI_RED_WALK_LEFT    16001
#define ID_ANI_RED_SHELL        16002   
#define ID_ANI_RED_SHELL_MOVING 16003   
#define ID_ANI_RED_SHELL_UPSET	16004
#define ID_ANI_RED_SHELL_UPSET_MOVING 16005   
#define ID_ANI_RED_SHELL_BACK        16006 
#define ID_ANI_RED_SHELL_UPSET_BACK	16007

#define ID_ANI_GREEN_WALK_RIGHT   16010
#define ID_ANI_GREEN_WALK_LEFT    16011
#define ID_ANI_GREEN_SHELL        16012   
#define ID_ANI_GREEN_SHELL_MOVING 16013   
#define ID_ANI_GREEN_SHELL_UPSET	16014
#define ID_ANI_GREEN_SHELL_UPSET_MOVING 16015   
#define ID_ANI_GREEN_SHELL_BACK        16016 
#define ID_ANI_GREEN_SHELL_UPSET_BACK	16017

#define ID_ANI_GREEN_WING_JUMP_RIGHT   16021
#define ID_ANI_GREEN_WING_JUMP_LEFT    16020

#define KOOPA_BBOX_WIDTH 15
#define KOOPA_BBOX_HEIGHT 24

#define KOOPA_SHELL_BBOX_WIDTH 16
#define KOOPA_SHELL_BBOX_HEIGHT 15

#define KOOPA_GREEN_WING_BBOX_WIDTH 15
#define KOOPA_GREEN_WING_BBOX_HEIGHT 26

#define KOOPA_STATE_WALKING       101   // Trạng thái di chuyển
#define KOOPA_STATE_SHELL         102   // Trạng thái mai rùa (normal / upset)
#define KOOPA_STATE_SHELL_MOVING       103   // Mai rùa move (normal / upset)
#define KOOPA_STATE_DIE           100   // SHELL  BIEN MAT KHI DÍNH ĐẠN
#define KOOPA_STATE_JUMP   104
#define KOOPA_STATE_HELD 105
#define KOOPA_STATE_DEAD_UPSIDE 106 // DIE KHI SHELL DUNG PHAI KOOPA
class CKoopa : public CGameObject
{
protected:
    float ax, ay;
    int type;
    ULONGLONG defend_start;      // Thời điểm vào mai
    ULONGLONG comeback_start;
    ULONGLONG turnaround_delay;
    ULONGLONG JumpTime;
    ULONGLONG deadDelay;
    bool isInShell; // mai 
    bool isUpset; // lat ngua
    bool isComeback;
    bool isHeld;
    bool isKicked;
    bool isOnPlatform;
    bool isTurning = false;
    bool isWing;


    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();

    int GetKoopaRedAniId();
    int GetKoopaGreenAniId();
    int GetKoopaGreenWingAniId();
    void OnNoCollision(DWORD dt);
    void OnCollisionWith(LPCOLLISIONEVENT e);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
public:
    CKoopa(float x, float y, int t);
    int IsUpset() { return isUpset; }

    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithBrickQues(LPCOLLISIONEVENT e);
    void OnCollisionWithBrickPSwitch(LPCOLLISIONEVENT e);
    void OnCollisionWithFireBall(LPCOLLISIONEVENT e);
    void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
    void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
    void OnCollisionWithPlantEnemies(LPCOLLISIONEVENT e);

    virtual int IsCollidable() { return 1; }
    virtual int IsBlocking() { return 1; }

    void SetHeld(bool held) { isHeld = held; }
    void SetState(int state);
    void SetIsUpset(int i) { this->isUpset = i; }
    void SetType(int j) { this->type = j; }

    int GetType() { return this->type; }

    bool IsWing() { return this->isWing; }
    bool IsHeld() const { return isHeld; }
    bool IsComeback() { return isComeback; }
    void UpdateWalkingOnPlatform(CPlatform* platform);
    void UpdateWalkingOnBrickPSwitch(CBrickPSwitch* brick);
    void UpdateWalkingOnEdge(float leftBound, float rightBound);

};

