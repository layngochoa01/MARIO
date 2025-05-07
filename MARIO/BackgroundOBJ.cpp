#include "BackgroundOBJ.h"
#include "BackgroundOBJ.h"

void CBackgroundOBJ::Render()
{

    CSprites* s = CSprites::GetInstance();
    s->Get(this->spriteIDBGOBJ)->Draw(x, y);
}

void CBackgroundOBJ::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = 0;
    t = 0;
    r = 0;
    b = 0;
}
