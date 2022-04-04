#pragma once
#include "CEntity.h" // FÜR SDL2 und die Restlichen Includes
class CMap
{
private:
	SDL_Rect position;
	SDL_Texture* texture;
	/*list <CMapEnity*> listeVonMapEntitys;*/
public:
	CMap(SDL_Texture* textureTemp, SDL_Rect coords);
};

