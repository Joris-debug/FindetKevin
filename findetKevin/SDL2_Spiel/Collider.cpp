#include "Collider.h"

Collider::Collider(SDL_Rect other)
{
	this->m_Box.w = other.w;
	this->m_Box.h = other.h;
	this->m_Box.x = other.x;
	this->m_Box.y = other.y;
}

bool Collider::checkCollison(Collider& other)
{
	SDL_Rect rect1 = this->m_Box;
	SDL_Rect rect2 = other.m_Box;
	//if (m_Box.x > other.m_Box.x && m_Box.x + m_Box.w < other.m_Box.x) return true;
	//if (m_Box.x > other.m_Box.x + other.m_Box.w && m_Box.x + m_Box.w < other.m_Box.x + other.m_Box.w) return true;
	//if (m_Box.y > other.m_Box.y && m_Box.y + m_Box.h < other.m_Box.y) return true;
	//if (m_Box.y > other.m_Box.y + other.m_Box.h && m_Box.y + m_Box.h < other.m_Box.y + other.m_Box.h) return true;

	if (rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.h + rect1.y > rect2.y) {
		return true;
	}

	return false;
}

void Collider::render(SDL_Renderer* renderer)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	SDL_RenderFillRect(renderer, &m_Box);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Collider::print()
{
	std::cout << m_Box.x << " " << m_Box.y << " " << m_Box.w << " " << m_Box.h << std::endl;
}