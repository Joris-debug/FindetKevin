#pragma once

#include <SDL.h>

#include "IKMap.h"
#include "Collider.h"

enum Direction
{
	LEFT,
	RIGHT,
	NONE
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
	float m_Inertia;
	bool m_CollidingRight;
	bool m_CollidingLeft;
	Direction m_Dir;
public:
	IKPlayer(IKMap* map);
	~IKPlayer();
public:
	void init();
	void update(double dt);
	void render();	// Uses the renderer of the map, to draw itself 
	void walk(Direction dir);
	Collider* checkCollision();
	Collider* checkCollision(Collider* col);
public:
	void jump();
private:
	bool tryMove();
	void applyVelocity();
};

