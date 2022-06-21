#pragma once

#include "CEntity.h"

#include <SDL.h>

#include "CGamemaster.h"

enum Direction
{
	RIGHT,
	LEFT,
	UP,
	DOWN
};

struct vec2
{
	float x;
	float y;

	vec2() = default;
	vec2(const vec2& other) = default;
	vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void addTo(SDL_Rect* rect) const
	{
		rect->x += this->x;
		rect->y += this->y;
	}
};

class Projectile : public CEntity
{
private:
	vec2 speed;
public:
	Projectile(CGamemaster* game, Direction dir, int startX, int startY, float speed);
	void update(int y, int x) override;
	int onInteract() override;
};

