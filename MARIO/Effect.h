#pragma once 
#include "GameObject.h"


#define SCORE_SPEED_UP 0.05f
#define SCORE_LIFETIME 1000
#define SCORE_GRAVITY 0.0005f

#define SCORE_BBOX_WIDTH 10
#define SCORE_BBOX_HEIGHT 16

#define EFFECT_SCORE_100 100
#define	EFFECT_SCORE_1000	1000

#define ID_ANI_SCORE_100	17000
#define ID_ANI_SCORE_1000	17001
class CEffect : public CGameObject
{
protected:
	float ay;
	bool isLive;
	ULONGLONG time_live_start;
	int type;

	int GetEffectAniId();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnNoCollision(DWORD dt);
public:
	CEffect(float x, float y, int t);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	bool IsLive() { return isLive; }
	void SetType(int t) { this->type = t; }
	int GetType() { return this->type; }

};

