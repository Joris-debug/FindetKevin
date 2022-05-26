#pragma once

#include <SDL.h>
#include "CPlayer.h"
#include "IKMap.h"

class IKGameLogic
{
private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
	SDL_Event m_Event;
	IKPlayer* m_Player;
	IKMap* m_Map;
public:
	void init();
	void update(double& dt);
	void render();
public:
	IKGameLogic(SDL_Renderer* renderer, SDL_Window* window);
	~IKGameLogic();
};

