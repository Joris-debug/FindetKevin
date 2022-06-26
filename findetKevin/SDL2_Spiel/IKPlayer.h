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
	b2Body* m_Wheel;
	b2WheelJoint* m_Spring;
private:
	SDL_Rect m_TextureCoords;
	SDL_Rect m_DstRect;
private:
	float m_Speed;
	float m_lastYValue;
	bool m_InAir;
	Direction m_Dir;
public:
	IKPlayer(IKMap* map);
	~IKPlayer();
public:
	void init();
	void update(double dt);
	void render();	// Uses the renderer of the map, to draw itself 
	inline b2Vec2 getPosB2() { return m_Body->GetPosition(); };
	inline float getLastY() { return m_lastYValue; };
	inline void setLastY(float y) { m_lastYValue = y; };
	inline SDL_Rect getDstRect() const { return m_DstRect; };
	Collider* checkCollision();
	Collider* checkCollision(Collider* col);
	inline int getHeight() { return m_Height; };
public:
	void walk(Direction dir);
	void jump();
private:
	bool tryMove();
	void applyVelocity();
	void animate(int x, int y);
};

