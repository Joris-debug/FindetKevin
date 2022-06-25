#pragma once

#include <SDL.h>

#include <iostream>

#include <box2d/box2d.h>

class Collider
{
private:
	int m_Width;
	int m_Height;
	b2Body* m_Body;
	b2World* m_Simulation;
public:
	int m_OffsetY;
	void render(SDL_Renderer* renderer, float yOffset, float sOffsetY);
	void update(float dt);
public:
	Collider(SDL_Rect other, b2World* simulation);
	Collider() = default;
	void print();
};