#pragma once

#include <SDL.h>

#include "IKMap.h"
#include "Collider.h"

enum Direction
{
	LEFT,
	RIGHT
};

class IKPlayer
{
private:
	SDL_Rect m_Bounds;
	SDL_Texture* m_Texture;
	IKMap* m_Map;
private:
	bool m_InAir;
	float m_VelocityX;
	float m_VelocityY;
public:
	IKPlayer(IKMap* map);
	~IKPlayer();
public:
	void init();
	void update(double dt);
	void render();	// Uses the renderer of the map, to draw itself 
	void walk(Direction dir);
	Collider* checkCollision();
public:
	void jump();
};

