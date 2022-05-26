#pragma once

#include <memory>

#include <SDL.h>

//#include "IKPlayer.h"

class IKPlayer;
class IKRenderLayer;

class IKMap
{
private:
	SDL_Renderer* m_Renderer;
	IKPlayer* m_Player;
	std::unique_ptr<IKRenderLayer> m_BackgroundLayer;
public:
	IKMap(SDL_Renderer* renderer);
	~IKMap();
	void init();
	void update(double dt);
	void render();
	inline SDL_Renderer* getRenderer() const { return m_Renderer; };
	inline IKPlayer* getPlayer() const { return m_Player; };
	inline IKRenderLayer* getCollisionLayer() const { return m_BackgroundLayer.get(); };
public:
	float m_Gravity;
};

