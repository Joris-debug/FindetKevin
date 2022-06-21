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
	int m_Width;
	int m_Height;
	int m_StartingX;
	int m_StartingY;
	SDL_Texture* m_Texture;
	IKMap* m_Map;
	b2Body* m_Body;
private:
	bool m_InAir;
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

