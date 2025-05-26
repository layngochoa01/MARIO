#include "BrickPSwitch.h"

void CBrickPSwitch::SetState(int s)
{
    switch (s) 
    {
    case BRICK_STATE_HAS_PSWITCH:
        isEmpty = false;
    case BRICK_STATE_NO_PSWITCH:
        isEmpty = true;
    }
    CGameObject::SetState(s);
}

void CBrickPSwitch::Render()
{
    if (!CheckObjectInCamera(this)) return;
    if( isEmpty )  CAnimations::GetInstance()->Get(ID_ANI_BRICK_EMPTY)->Render(x, y);
    else  CAnimations::GetInstance()->Get(ID_ANI_BRICK_P_SWITCH)->Render(x, y);
   
      RenderBoundingBox();
}

void CBrickPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBrickPSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - BRICK_PSWITCH_BBOX_WIDTH / 2;
    t = y - BRICK_PSWITCH_BBOX_HEIGHT / 2;
    r = l + BRICK_PSWITCH_BBOX_WIDTH;
    b = t + BRICK_PSWITCH_BBOX_HEIGHT;
}

void CBrickPSwitch::OnNoCollision(DWORD dt)
{
    vx = vy = 0;
}
