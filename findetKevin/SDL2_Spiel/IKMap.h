#pragma once

#include <memory>

#include <SDL.h>

#include <box2d/box2d.h>

//#include "IKPlayer.h"

class IKPlayer;
class IKRenderLayer;

class IKMap
{
private:
	SDL_Renderer* m_Renderer;
	IKPlayer* m_Player;
	std::unique_ptr<IKRenderLayer> m_BackgroundLayer;
	float m_OffsetY;
	b2World* m_Simulation;
public:
	float m_Gravity;
public:
	IKMap(SDL_Renderer* renderer);
	~IKMap();
	void init();
	void update(double dt);
	void render();
	inline SDL_Renderer* getRenderer() const { return m_Renderer; };
	inline IKPlayer* getPlayer() const { return m_Player; };
	inline IKRenderLayer* getCollisionLayer() const { return m_BackgroundLayer.get(); };
	inline b2World* getSimulation() const { return m_Simulation; };
	inline float getOffsetY() { return m_OffsetY; }
	inline void setOffsetY(float newOffset) { m_OffsetY = newOffset; }
};

