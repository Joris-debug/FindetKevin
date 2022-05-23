#pragma once

#include <SDL.h>

#include "IKMap.h"

enum Direction
{
	LEFT,
	RIGHT
};

class IKPlayer
{
private:
	SDL_Rect m_Bounds;
	SDL_Texture* m_Texture;
	IKMap* m_Map;
public:
	IKPlayer(IKMap* map);
	void init();
	void render();	// Uses the renderer of the map, to draw itself 
	void walk(Direction dir);
};

