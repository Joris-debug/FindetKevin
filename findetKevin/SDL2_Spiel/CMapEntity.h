#pragma once
#include "CEntity.h" // F�R SDL2 und die Restlichen Includes
class CMapEntity
{
private:
	SDL_Rect bounds;
public:
	void onInteract();
	CMapEntity(SDL_Rect bounds);
	SDL_Rect getBounds();
};

