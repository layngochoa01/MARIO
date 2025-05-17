#pragma once
#include "GameObject.h"
#include "Platform.h"

#define KOOPA_GRAVITY         0.0005f    
#define KOOPA_WALK_SPEED      0.02f     // Tốc độ di chuyển
#define KOOPA_KICKED_SPEED    0.18f     // Tốc độ khi bị đá
#define KOOPA_JUMP_DEFLECT    0.4f      // Lực nảy khi bị giết


#define KOOPA_DEFEND_TIMEOUT  8000      // Thời gian ở trong mai (ms)
#define KOOPA_COMEBACK_TIME   6000      // Thời gian trước khi hồi phục
#define KOOPA_TIME_DELAY_AROUND 100

#define KOOPA_TYPE_RED  1

#define ID_ANI_RED_WALK_RIGHT   16000
#define ID_ANI_RED_WALK_LEFT    16001
#define ID_ANI_RED_SHELL        16002   // Mai rùa đứng yên
#define ID_ANI_RED_SHELL_MOVING 16003   // Mai rùa đang lăn
#define ID_ANI_RED_SHELL_UPSET	16004
#define ID_ANI_RED_SHELL_UPSET_MOVING 16005   // Mai rùa đang lăn
#define ID_ANI_RED_SHELL_BACK        16006 
#define ID_ANI_RED_SHELL_UPSET_BACK	16007

#define KOOPA_BBOX_WIDTH 15
#define KOOPA_BBOX_HEIGHT 25

#define KOOPA_SHELL_BBOX_WIDTH 16
#define KOOPA_SHELL_BBOX_HEIGHT 15

#define KOOPA_STATE_WALKING       1   // Trạng thái di chuyển
#define KOOPA_STATE_SHELL         2   // Trạng thái mai rùa (normal / upset)
#define KOOPA_STATE_SHELL_MOVING       3   // Mai rùa move (normal / upset)
#define KOOPA_STATE_DIE           10   // Chết KHI SHELL CHẠM GOOMPA OR DÍNH ĐẠN

class CKoopa : public CGameObject
{
protected:
    float ax, ay;
    int type;
    ULONGLONG defend_start;      // Thời điểm vào mai
    ULONGLONG comeback_start;
    ULONGLONG turnaround_delay;
    bool isInShell; // mai 
    bool isUpset; // lat ngua
    bool isComeback;
    bool isHeld;
    bool isKicked;
    bool isOnPlatform;
    bool isTurning = false;


    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    int GetKoopaRedAniId();
    void OnNoCollision(DWORD dt);
    void OnCollisionWith(LPCOLLISIONEVENT e);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
public:
    CKoopa(float x, float y, int t);
    int IsUpset() { return isUpset; }

    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithBrickQues(LPCOLLISIONEVENT e);
    void OnCollisionWithFireBall(LPCOLLISIONEVENT e);
    void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
    void OnCollisionWithKoopa(LPCOLLISIONEVENT e);

    virtual int IsCollidable() { return 1; }
    virtual int IsBlocking() { return 1; }

    void SetState(int state);
    void SetIsUpset(int i) { this->isUpset = i; }
    int GetType() { return this->type; }
    void UpdateWalkingOnPlatform(CPlatform* platform);

};

