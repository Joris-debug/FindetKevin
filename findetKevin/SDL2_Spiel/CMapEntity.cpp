#include "CMapEntity.h"

CMapEntity::CMapEntity(SDL_Rect bounds)
{
	this->bounds = bounds;
}

SDL_Rect CMapEntity::getBounds()
{
	return bounds;
}
