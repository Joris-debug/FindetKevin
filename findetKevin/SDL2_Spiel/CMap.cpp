#include "CMap.h"
CMap::CMap(SDL_Texture* textureTemp, SDL_Rect coords)
{
	position = coords;
	texture = textureTemp;
}