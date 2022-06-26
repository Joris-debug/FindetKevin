#pragma once

#include <SDL.h>
#include "IKRenderLayer.h"

class IKVirus
{
private:
	SDL_Rect m_Bounds;
	SDL_Rect m_DstRect;
	SDL_Rect m_TextureCoords;
	SDL_Texture* m_Texture;
	SDL_Renderer* m_Renderer;
	IKRenderLayer* m_ParentLayer;
	int m_AnimationSpeed;
	int m_FrameCounter;
public:
	static int s_KilledViruses;
public:
	IKVirus(IKRenderLayer* layer, SDL_Rect bounds, SDL_Renderer* renderer);
	void render();
	int update();
	void gettingKilled();
};

