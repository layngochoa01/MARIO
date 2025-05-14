#include "BrickQues.h"
#include "Coin.h"
#include "PlayScene.h"

#include "Debug.h"

void CBrickQues::Render()
{
    if (!CheckObjectInCamera(this)) return;
    int aniId = isEmpty ? ID_ANI_BRICK_EMPTY : ID_ANI_BRICK_QUES;
    
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    RenderBoundingBox();
}

void CBrickQues::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
        vy += ay * dt;
        if (y <= minY)
        {
            vy = BRICK_QUES_SPEED_DOWN;
        }
        if (y > startY + BRICK_BBOX_HEIGHT - JUMP_OFFSET)
        {
            y = startY;
            vy = 0;
            isEmpty = true;
            isUnbox = true;
        }
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrickQues::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    CBrick::GetBoundingBox(l, t, r, b);
}

void CBrickQues::OnNoCollision(DWORD dt)
{
    y += vy * dt;
}

void CBrickQues::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case  BRICK_QUES_STATE_UP:
        vy = -BRICK_QUES_SPEED_UP;
        break;

    }
}
