﻿#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}

CGameObject::~CGameObject()
{

}

bool CGameObject::CheckObjectInCamera(CGameObject* obj)
{

	float camX, camY;
	CGame::GetInstance()->GetCamPos(camX, camY);
	// Giới hạn camera theo tọa độ center
	float camLeft = camX;
	float camTop = camY;
	float camRight = camLeft + SCREEN_WIDTH;
	float camBottom = camTop + SCREEN_HEIGHT;

	// Bounding box của object
	float left, top, right, bottom;
	obj->GetBoundingBox(left, top, right, bottom);

	bool isInCamera = (
		right >= camLeft &&
		left <= camRight &&
		bottom >= camTop &&
		top <= camBottom
		);

	return isInCamera;
}
