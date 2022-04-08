#include "CEnemy.h"

CEnemy::CEnemy(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, int healthpoints, int damage, int walkingFrames, int attackFrames, int idleFrames, int deathFrames) : CEntity(textureTemp, tag, bounds, textureCoords)
{
	this->healthpoints = healthpoints;
	this->damage = damage;
	this->walkingFrames = walkingFrames;
	this->attackFrames = attackFrames;
	this->idleFrames = idleFrames;
	this->deathFrames = deathFrames;
}

void CEnemy::update(int y, int x)
{
    int totalFrames = 6;   // Animation besteht jeweils aus 6 sprites
    int delayPerFrame = 100;
    int movingDirection = 0;

    if (x > 0)
        movingDirection = 0; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
    else
        movingDirection = 6;// Anfangsprite ist drei weiter Rechts auf dem Spritesheet

    int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
    textureCoords.x = frame * textureCoords.w;
    textureCoords.y = 16;
    if (y == 0 && x == 0)
    {
        textureCoords.x = 0;
        textureCoords.y = 0;
        textureCoords.h = 16;
        textureCoords.w = 16;
    }
}