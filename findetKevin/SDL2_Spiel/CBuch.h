#pragma once
#include "CNPC.h"
class CBuch : public CNPC
{
public:
	CBuch(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
	int onInteract();
};


