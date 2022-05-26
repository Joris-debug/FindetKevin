#pragma once

#include <string>
#include <list>

#include <SDL.h>

#include "IKMap.h"
#include "Collider.h"

class IKRenderLayer
{
private:
	SDL_Texture* m_Image;
	SDL_Rect m_SrcRect;
	SDL_Rect m_DstRect;
	SDL_Renderer* m_Renderer;
	float m_Distance;
	std::list<Collider*> m_Colliders;
public:
	void init(const std::string& path, SDL_Renderer* renderer, float distance);
	void update(double dt);
	void render(bool renderCols);
public:
	inline std::list<Collider*>& getColliders() { return m_Colliders; }
public:
	IKRenderLayer() = default;
	~IKRenderLayer();


};

