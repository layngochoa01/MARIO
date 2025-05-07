#pragma once
#include "GameObject.h"
class CBackgroundOBJ : public CGameObject
{
protected:
    int spriteIDBGOBJ;
public:
    CBackgroundOBJ(float x, float y, int sprite_id) : CGameObject(x, y)
    {
        this->spriteIDBGOBJ = sprite_id;
    }
    void Render();
    void Update(DWORD dt) {}
    void GetBoundingBox(float& l, float& t, float& r, float& b);
};

