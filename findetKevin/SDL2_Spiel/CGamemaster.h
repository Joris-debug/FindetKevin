#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
#include <list>
#include "CEntity.h"
#include "CPlayer.h"
#include "CMap.h"
class CGamemaster
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event input;
	list <CEntity*> listeVonEntitys;
	CMap* currentMap;
	CMap* currentMap_TopLayer;
	CPlayer * spielerPointer;
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
private:
	void gameLoop();
public:
	CGamemaster();
	void init();
};

