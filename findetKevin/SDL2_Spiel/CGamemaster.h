#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
class CGamemaster
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event input;

public:
	CGamemaster();
	void GameLoop();
};

