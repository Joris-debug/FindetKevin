#pragma once
#include "CEntity.h"
class CEnemy :    public CEntity
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
public:
	void shoot();
	int onInteract();
	int getHealth();
	void setHealth(int healthpoints);
	int getMaxHealth();
	void update(int y, int x);
	CEnemy(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving, int healthpoints, int damage, int walkingFrames, int attackFrames, int idleFrames, int deathFrames);
};

