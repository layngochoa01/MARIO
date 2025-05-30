#include "BrickPSwitch.h"
#include "PlayScene.h"
void CBrickPSwitch::SetState(int s)
{
    switch (s)
    {
    case BRICK_STATE_HAS_MODEL:
    {
        vy = 0;
        isEmpty = false;
        break;
    }    
    case BRICK_STATE_NO_PSWITCH: 
        {
        isEmpty = true; 
        break;
        }
    case BRICK_STATE_COIN:
    {
        isTransformedToCoin = true;
        break;
    }
    case  BRICK_STATE_BREAK: 
        {//danh cho bi raccoon tan cong / mai rua dap vao
        isBroken = true;
        break;
        }
    case BRICK_MODEL_STATE_UP:
    {
        vy = -BRICK_MODEL_SPEED_UP;
        isStateUpDown = true;
        break;   
    }    
    }
    CGameObject::SetState(s);
}

void CBrickPSwitch::CreatePSwitch()
{
    if (PS != nullptr) return; // tránh tạo lại
    PS = new CPSwitch(x, y);  
    PS->SetState(PSWITCH_STATE_RISING);
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    scene->PushObject(PS);  
}

void CBrickPSwitch::Render()
{
    if (!CheckObjectInCamera(this)) return;
    int aniId = ID_ANI_BRICK_MODEL;

    if (model == MODEL_PSWITCH)
    {
        aniId = isEmpty ? ID_ANI_BRICK_EMPTY : ID_ANI_BRICK_MODEL;
    }
    else if (model == MODEL_COIN)
    {
        aniId = isTransformedToCoin ? ID_ANI_COIN_SWITCH : ID_ANI_BRICK_MODEL;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
     RenderBoundingBox();
}

void CBrickPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if ( PS != nullptr)  DebugOut(L"[PS ACTIVE]\n");
    if (model == MODEL_COIN && PS != nullptr)
    {
        DebugOut(L"PS ACTIVE %d, FINISH %d \n\n", PS->IsActivated(), PS->IsFinish());

        if (PS->IsActivated() && !isTransformedToCoin)
        {
            SetState(BRICK_STATE_COIN);
        }
        else if (PS->IsFinish() && isTransformedToCoin)
        {
            SetState(BRICK_STATE_HAS_MODEL); // Biến lại thành brick
        }

    }
 
    if (isStateUpDown)
    {
        DebugOut(L"[BrickPSwitch] y = %.2f, vy = %.2f, minY = %.2f, startY = %.2f\n", y, vy, minY, startY);
        y += vy * dt;

        if (y <= minY)
        {
            vy = BRICK_MODEL_SPEED_DOWN;
        }

        if (y >= startY)
        {
            y = startY;
            vy = 0;
            isStateUpDown = false;
            SetState(BRICK_STATE_HAS_MODEL);
        }
    }
    
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrickPSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - BRICK_MODEL_BBOX_WIDTH / 2;
    t = y - BRICK_MODEL_BBOX_HEIGHT / 2;
    r = l + BRICK_MODEL_BBOX_WIDTH;
    b = t + BRICK_MODEL_BBOX_HEIGHT;
}

void CBrickPSwitch::OnNoCollision(DWORD dt)
{  
    if (isStateUpDown) vx = 0;
    else vx = vy = 0;
}
