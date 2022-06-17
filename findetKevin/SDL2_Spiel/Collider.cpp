#include "Collider.h"

#include <SDL.h>

Collider::Collider(SDL_Rect other)
{
	this->m_Box.w = other.w;
	this->m_Box.h = other.h;
	this->m_Box.x = other.x;
	this->m_Box.y = other.y;

	this->m_OffsetY = 0;
}

bool Collider::checkCollison(Collider& other)
{
	SDL_Rect rect1 = this->m_Box;
	SDL_Rect rect2 = other.m_Box;

	int o1 = this->m_OffsetY;
	int o2 = other.m_OffsetY;

	rect1.y += o1;
	rect2.y += o2;

	if (SDL_HasIntersection(&rect1, &rect2) == SDL_TRUE)
	{
		return true;
	}

	return false;
}

void Collider::render(SDL_Renderer* renderer)
{
	Uint8 r, g, b, a;

	SDL_Rect renderBox{ m_Box.x, m_Box.y + m_OffsetY, m_Box.w, m_Box.h };

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 100, 0, 100, 0);
	SDL_RenderFillRect(renderer, &renderBox);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

// lol

void Collider::print()
{
	//std::cout << m_Box.x << " " << m_Box.y << " " << m_Box.w << " " << m_Box.h << std::endl;
}