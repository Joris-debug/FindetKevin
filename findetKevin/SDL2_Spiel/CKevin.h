#pragma once
#include "CNPC.h"
class CKevin :
    public CNPC
{
public:
    CKevin(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
    int onInteract();
};

