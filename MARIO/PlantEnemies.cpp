#include "PlantEnemies.h"
#include "Mario.h"
#include "FireBall.h"
#include "PlayScene.h"

int CPlantEnemies::GetPlantAniId()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mx, my;
	mario->GetPosition(mx, my);

	bool isLeft = mx < x;
	bool isTop = my < y;

	if (!isShooting) 
	{
		if (isLeft && isTop) return ID_ANI_PLANT_UP_LEFT_TOP;
		else if (isLeft && !isTop) return ID_ANI_PLANT_UP_LEFT_BOTTOM;
		else if (!isLeft && isTop) return ID_ANI_PLANT_UP_RIGHT_TOP;
		else return ID_ANI_PLANT_UP_RIGHT_BOTTOM;
	}
	else 
	{
		if (isLeft && isTop) return ID_ANI_PLANT_SHOOT_LEFT_TOP;
		else if (isLeft && !isTop) return ID_ANI_PLANT_SHOOT_LEFT_BOTTOM;
		else if (!isLeft && isTop) return ID_ANI_PLANT_SHOOT_RIGHT_TOP;
		else return ID_ANI_PLANT_SHOOT_RIGHT_BOTTOM;
	}
}

CPlantEnemies::CPlantEnemies(float x, float y) : CGameObject(x, y)
{
	this->startY = y;
	this->minY = startY - PLANT_BBOX_HEIGHT;
	
	isUpping = false ;
	isDowning = false ;
	isShooting = false;

	SetState(PLANT_STATE_UP);

}

void CPlantEnemies::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PLANT_BBOX_WIDTH / 2;
	top = y - PLANT_BBOX_HEIGHT / 2;
	right = left + PLANT_BBOX_WIDTH;
	bottom = top + PLANT_BBOX_HEIGHT;
}

void CPlantEnemies::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CheckObjectInCamera(this)) return;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsGrowing() || mario->IsRaccoon() || mario->GetState() == MARIO_STATE_DIE) return;
	
	
	float mx, my;
	mario->GetPosition(mx, my);

	if (state == PLANT_STATE_UP)
	{
		if (y > minY)
		{
			vy = -PLANT_SPEED_UP_DOWN;
		}
		else 
		{
			if(time_shoot == 0) time_shoot = GetTickCount64();
			vy = 0;
			y = minY;

			if (!isShooting && (GetTickCount64() - time_shoot) > TIME_SHOOT)
			{
				float deltaX = mx - x;
				float deltaY = my - y;
				float dx = (deltaX > 0) ? deltaX : -deltaX;
				float dy = (deltaY > 0) ? deltaY : -deltaY;

				if (dx > MARIO_SAFE_RADIUS_X || dy > MARIO_SAFE_RADIUS_Y)
				{
					ShootFireBall();
					isShooting = true;
				}
			}

			if (GetTickCount64() - time_shoot > TIME_OUT_PIPE) {
				SetState(PLANT_STATE_DOWN);
			}
		}
	}
	else if (state == PLANT_STATE_DOWN) 
	{
		
		if (y < startY + 2) 
		{
			vy = PLANT_SPEED_UP_DOWN;
			y += vy * dt;
			//DebugOut(L"vy = %f, y = %f, startY = %f\n", vy, y, startY);

		}
			
		else 
		{
			y = startY + 2;
			vy = 0;
			isDowning = false;

			if (GetTickCount64() - time_down > TIME_DOWN_PIPE)
			{
				SetState(PLANT_STATE_UP);
				time_down = 0;
			}

		}

	
		
	}

	//DebugOut(L"isShooting %d \n",isShooting);
	//DebugOut(L"vy = %f\n", vy);
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPlantEnemies::Render()
{
	if (!CheckObjectInCamera(this)) return;
	int aniId = GetPlantAniId();
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CPlantEnemies::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	DebugOut(L"[OnNoCollision] vy = %f, dt = %d, y = %f\n", vy, dt, y);
}

void CPlantEnemies::SetState(int state)
{
	

	if (state == PLANT_STATE_UP) 
	{
		vy = -PLANT_SPEED_UP_DOWN;  
		isUpping = true;
		isDowning = false;
		isShooting = false;           
		time_shoot = 0;      
		time_down = 0;
	}
	else if (state == PLANT_STATE_DOWN)
	{
		vy = PLANT_SPEED_UP_DOWN;
		isDowning = true; 
		isUpping = false;
		time_shoot = 0;
		isShooting = false;
		time_down = GetTickCount64();
	}
	CGameObject::SetState(state);
}

void CPlantEnemies::ShootFireBall()
{
	if (isShooting) return;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mx, my;
	mario->GetPosition(mx, my);
	CFireBall* fb = new CFireBall(x , y - PLANT_BBOX_HEIGHT/4, mx, my);
	scene->PushObject(fb);
	isShooting = true;
	
	//DebugOut(L"[ShootFireBall] Fireball created at (%f, %f), vx = %f, vy = %f\n", mx, my);

}