#pragma once
#include "GameObject.h"


#define PLANT_VENUS_RED_BBOX_WIDTH 16
#define PLANT_VENUS_RED_BBOX_HEIGHT 32

#define PLANT_VENUS_GREEN_BBOX_WIDTH 16
#define PLANT_VENUS_GREEN_BBOX_HEIGHT 24

#define PLANT_PIRANHA_GREEN_BBOX_WIDTH 15
#define PLANT_PIRANHA_GREEN_BBOX_HEIGHT 24

#define PLANT_SPEED_UP_DOWN 0.05f
#define TIME_OUT_PIPE 3000
#define TIME_DOWN_PIPE 5000
#define TIME_SHOOT 1000

#define MARIO_SAFE_RADIUS_X 24.0f
#define MARIO_SAFE_RADIUS_Y 40.0f 

#define PLANT_STATE_UP 0
#define PLANT_STATE_DOWN 1

#define ID_ANI_VENUS_RED_UP_LEFT_TOP 12200
#define ID_ANI_VENUS_RED_SHOOT_LEFT_TOP 12201

#define ID_ANI_VENUS_RED_UP_LEFT_BOTTOM 12202
#define ID_ANI_VENUS_RED_SHOOT_LEFT_BOTTOM 12203

#define ID_ANI_VENUS_RED_UP_RIGHT_TOP 12204
#define ID_ANI_VENUS_RED_SHOOT_RIGHT_TOP 12205

#define ID_ANI_VENUS_RED_UP_RIGHT_BOTTOM 12206
#define ID_ANI_VENUS_RED_SHOOT_RIGHT_BOTTOM 12207

#define ID_ANI_PIRANHA_GREEN 12210

#define ID_ANI_VENUS_GREEN_UP_LEFT_TOP 12220
#define ID_ANI_VENUS_GREEN_SHOOT_LEFT_TOP 12221

#define ID_ANI_VENUS_GREEN_UP_LEFT_BOTTOM 12222
#define ID_ANI_VENUS_GREEN_SHOOT_LEFT_BOTTOM 12223

#define ID_ANI_VENUS_GREEN_UP_RIGHT_TOP 12224
#define ID_ANI_VENUS_GREEN_SHOOT_RIGHT_TOP 12225

#define ID_ANI_VENUS_GREEN_UP_RIGHT_BOTTOM 12226
#define ID_ANI_VENUS_GREEN_SHOOT_RIGHT_BOTTOM 12227


#define MODEL_VENUS_RED 0
#define MODEL_VENUS_GREEN 1
#define MODEL_PIRANHA_GREEN 2

class CPlantEnemies : public CGameObject
{
	protected:
		float startY;
		float minY;
		int model;
		bool isUpping;
		bool isDowning ;
		bool isShooting ;

		ULONGLONG time_out_pipe = 0;
		ULONGLONG time_down = 0;
		ULONGLONG time_shoot = 0;

		void ShootFireBall(float h);
		int GetPlantVenusRedAniId();
		int GetPlantVenusGreenAniId();
		int GetPlantPiranhaGreenAniId();
		
		virtual int IsCollidable() { return 1; };
		virtual int IsBlocking() { return 0; }
		bool IsEnemy() { return true; }
	public:
		CPlantEnemies(float x, float y, int m) ;
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) ;
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) ;
		virtual void Render() override;
		virtual void OnNoCollision(DWORD dt);
		virtual void SetState(int state) ;
};

