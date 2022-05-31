#pragma once

#include <SDL.h>

#include <iostream>

class Collider
{
private:
	SDL_Rect m_Box;
public:
	int m_OffsetY;
	bool checkCollison(Collider& other);
	void render(SDL_Renderer* renderer);
public:
	Collider(SDL_Rect other);
	Collider() = default;
public:
	inline void setRect(SDL_Rect* rect) { std::memcpy(&m_Box, rect, sizeof(SDL_Rect)); }
	inline SDL_Rect& getRect() { return m_Box; }
	void print();
};