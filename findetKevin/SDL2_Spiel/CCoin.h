#pragma once
#include "CEntity.h"
class CCoin :   public CEntity
{
public:
	CCoin(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
	int onInteract();
	void update(int y, int x);
	inline void entityPathfinding(double deltaTime) { return; }	//Münzen bewegen sich nicht
};

