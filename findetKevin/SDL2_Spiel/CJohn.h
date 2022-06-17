#pragma once
#include "CNPC.h"
#include "CEntity.h"
class CJohn : public CNPC
{
public:
	CJohn(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
	int onInteract();
};

