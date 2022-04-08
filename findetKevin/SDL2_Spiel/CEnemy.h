#pragma once
#include "CEntity.h"
class CEnemy :    public CEntity
{
private:
	int healthpoints;
	Uint32 timeSinceLastShot;
	int damage;
	int walkingFrames;
	int attackFrames;
	int idleFrames;
	int deathFrames;
public:
	void shoot();
	void update(int y, int x);
	CEnemy(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, int healthpoints, int damage, int walkingFrames, int attackFrames, int idleFrames, int deathFrames);
};

