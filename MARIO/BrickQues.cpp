#include "BrickQues.h"
#include "Coin.h"
#include "PlayScene.h"

#include "Debug.h"

void CBrickQues::Render()
{
    if (!CheckObjectInCamera(this)) return;
    int aniId = isEmpty ? ID_ANI_BRICK_EMPTY : ID_ANI_BRICK_QUES;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    //RenderBoundingBox();
}

void CBrickQues::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!CheckObjectInCamera(this)) return;
    if (x != startX) {
        x = startX;
    }

    if (!isEmpty) {
        if (y != startY) y = startY;
        if (x != startX) x = startX;
    }
    if (isUnbox) {
        vy = 0;
        ay = 0;
        vx = 0;
        y = startY;
        x = startX;
    }
    else {
        y += vy * dt;
        if (y <= minY)
        {
            vy = BRICK_QUES_SPEED_DOWN;
        }
        else if (y > startY + (BRICK_BBOX_HEIGHT)  - JUMP_OFFSET)
        {
            y = startY;
            isUnbox = true;
            SetState(-1);
        }
    }
   // DebugOut(L"[BRICK QUES ] X %f, Y %f, VX %f, VY %f, isEmpty %d, isUnbox %d \n", x, y, vx, vy, isEmpty ,isUnbox);
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrickQues::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - BRICK_QUES_BBOX_WIDTH / 2;
    t = y - BRICK_QUES_BBOX_HEIGHT / 2;
    r = l + BRICK_QUES_BBOX_WIDTH;
    b = t + BRICK_QUES_BBOX_HEIGHT;
   // DebugOut(L"[BRICKQ] bbox: %f %f %f %f\n", l, t, r, b);
}

void CBrickQues::OnNoCollision(DWORD dt)
{
   /* if (state == BRICK_QUES_STATE_UP) vx = 0;
    else vx = vy = 0;
    
    */
}

void CBrickQues::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case  BRICK_QUES_STATE_UP:
        vy = -BRICK_QUES_SPEED_UP;
        break;
    default: 
        vy = 0;
        break;
    }
}
