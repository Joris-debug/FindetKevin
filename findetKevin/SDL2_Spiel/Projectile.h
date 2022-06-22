#pragma once

#include "CEntity.h"

#include <SDL.h>

#include "CGamemaster.h"

#include <cmath>

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

	vec2 operator-(const vec2& other)
	{
		return { this->x - other.x, this->y - other.y };
	}

	void addTo(SDL_Rect* rect) const
	{
		rect->x += this->x;
		rect->y += this->y;
	}

	float getXDist(const vec2& other)
	{
		float dist = this->x - other.x;
		if (dist < 0)
			dist *= -1;
		return dist;
	}

	float getYDist(const vec2& other)
	{
		float dist = this->y - other.y;
		if (dist < 0)
			dist *= -1;
		return dist;
	}

	float getDist(const vec2& other)
	{
		vec2 t_o = *(this) - other;
		float dist = sqrt((t_o.x * t_o.x) + (t_o.y * t_o.y));
		return dist;
	}
};

class Projectile : public CEntity
{
private:
	vec2 speed;
	vec2 startPos;
	float maxTravelDist;
public:
	Projectile(CGamemaster* game, Direction dir, int startX, int startY, float speed, float maxDist);
	void update(int y, int x) override;
	int onInteract() override;
};

