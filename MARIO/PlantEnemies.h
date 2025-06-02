#pragma once
#include "GameObject.h"


#define PLANT_BBOX_WIDTH 16
#define PLANT_BBOX_HEIGHT 32

#define PLANT_SPEED_UP_DOWN 0.05f
#define TIME_OUT_PIPE 3000
#define TIME_DOWN_PIPE 3000
#define TIME_SHOOT 1000

#define MARIO_SAFE_RADIUS_X 10.0f
#define MARIO_SAFE_RADIUS_Y 10.0f 

#define PLANT_STATE_UP 0
#define PLANT_STATE_DOWN 1

#define ID_ANI_PLANT_UP_LEFT_TOP 12200
#define ID_ANI_PLANT_SHOOT_LEFT_TOP 12201

#define ID_ANI_PLANT_UP_LEFT_BOTTOM 12202
#define ID_ANI_PLANT_SHOOT_LEFT_BOTTOM 12203

#define ID_ANI_PLANT_UP_RIGHT_TOP 12204
#define ID_ANI_PLANT_SHOOT_RIGHT_TOP 12205

#define ID_ANI_PLANT_UP_RIGHT_BOTTOM 12206
#define ID_ANI_PLANT_SHOOT_RIGHT_BOTTOM 12207

class CPlantEnemies : public CGameObject
{
	protected:
		float startY;
		float minY;

		bool isUpping;
		bool isDowning ;
		bool isShooting ;

		ULONGLONG time_out_pipe = 0;
		ULONGLONG time_down = 0;
		ULONGLONG time_shoot = 0;

		void ShootFireBall();
		int GetPlantAniId();

		virtual int IsCollidable() { return 1; };
		virtual int IsBlocking() { return 0; }

	public:
		CPlantEnemies(float x, float y) ;
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) ;
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) ;
		virtual void Render() override;
		virtual void OnNoCollision(DWORD dt);
		virtual void SetState(int state) ;
};

