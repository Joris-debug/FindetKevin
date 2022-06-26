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
	std::unique_ptr<IKRenderLayer> m_OrganLayer;
	std::unique_ptr<IKRenderLayer> m_BorderLayer;
	std::unique_ptr<IKRenderLayer> m_GeometryLayer;
	std::unique_ptr<IKRenderLayer> m_Membran1Layer;
	std::unique_ptr<IKRenderLayer> m_BackgroundLayer;
	std::unique_ptr<IKRenderLayer> m_TopBlockerLayer;
	std::unique_ptr<IKRenderLayer> m_BottomBlockerLayer;
	float m_OffsetY;
	float m_StartingOffsetY;
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
	inline IKRenderLayer* getCollisionLayer() const { return m_GeometryLayer.get(); };
	inline b2World* getSimulation() const { return m_Simulation; };
	inline float getOffsetY() { return m_OffsetY; };
	inline void setOffsetY(float newOffset) { m_OffsetY = newOffset; };
	inline float getStartingOffsetY() const { return m_StartingOffsetY; };
};

