#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
#include <list>
#include "CEntity.h"
#include "CPlayer.h"
class CGamemaster
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event input;
	list <CEntity*> listeVonEntitys;
	CPlayer * spielerPointer;
private:
	void gameLoop();
public:
	CGamemaster();
	void init();
};

