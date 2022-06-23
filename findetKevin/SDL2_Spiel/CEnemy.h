#pragma once
#include "CEntity.h"

#include "Projectile.h"

class CEnemy : public CEntity
{
private:
	int maxHealthpoints;
	int healthpoints;
	Uint32 timeSinceLastShot;
	int damage;
	int walkingFrames;
	int attackFrames;
	int idleFrames;
	int deathFrames;
	int cooldown;
	int framesAfterLastAttack;
public:
	//void shoot();
	void shouldShoot();
	int onInteract();
	int getHealth();
	void setHealth(int healthpoints);
	int getMaxHealth();
	void update(int y, int x);
	CEnemy(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving, int healthpoints, int damage, int walkingFrames, int attackFrames, int idleFrames, int deathFrames);
public:
	inline void setCooldown(int c) { this->cooldown = c; };
};

