#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include <list>
using namespace std;
class CEntity
{
protected:
	SDL_Rect bounds;
	SDL_Rect textureCoords;
	SDL_Texture * texture;
	string tag;
public:
	virtual void update();
	void renderer(SDL_Renderer* renderer);
	CEntity(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords);
	~CEntity();
	CEntity();
};

