#pragma once

#include <SDL.h>

//#include "IKPlayer.h"

class IKPlayer;

class IKMap
{
private:
	SDL_Renderer* m_Renderer;
	IKPlayer* m_Player;
public:
	IKMap(SDL_Renderer* renderer);
	~IKMap();
	void init();
	void render();
	inline SDL_Renderer* getRenderer() const { return m_Renderer; };
	inline IKPlayer* getPlayer() const { return m_Player; };
};

