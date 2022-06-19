#pragma once

#include <SDL.h>

#include <iostream>

#include <box2d/box2d.h>

class Collider
{
private:
	SDL_Rect m_Box;
	b2Body* m_Body;
	b2World* m_Simulation;
public:
	int m_OffsetY;
	bool checkCollison(Collider& other);
	void render(SDL_Renderer* renderer);
	void update(float dt);
public:
	Collider(SDL_Rect other, b2World* simulation);
	Collider() = default;
public:
	inline void setRect(SDL_Rect* rect) { std::memcpy(&m_Box, rect, sizeof(SDL_Rect)); }
	inline SDL_Rect& getRect() { return m_Box; }
	void print();
};