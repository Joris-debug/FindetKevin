#pragma once

#include <string>
#include <list>

#include <SDL.h>

#include "IKMap.h"
#include "Collider.h"
//#include "IKVirus.h"

class IKVirus;

class IKRenderLayer
{
private:
	SDL_Texture* m_Image;
	SDL_Rect m_SrcRect;
	SDL_Rect m_DstRect;
	SDL_Renderer* m_Renderer;
	float m_Distance;
	std::list<Collider*> m_Colliders;
	std::list<IKVirus*> m_Viruses;
	IKMap* m_Map;
	float m_PersOffsetY;
public:
	void init(const std::string& path, SDL_Renderer* renderer, float distance, IKMap* map, int height, float yOffset);
	void update(double dt);
	void render(bool renderCols);
public:
	inline std::list<Collider*>& getColliders() { return m_Colliders; }
	inline std::list<IKVirus*>& getViruses() { return m_Viruses; }
	inline IKMap* getMap() const { return m_Map; };
	inline float getPersOffsetY() const { return m_PersOffsetY; };
	inline float getDistance() const { return m_Distance; };
public:
	IKRenderLayer() = default;
	~IKRenderLayer();


};

