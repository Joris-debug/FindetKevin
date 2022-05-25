#include "CEntity.h"
#include "CGamemaster.h"
int CEntity::entityCount = 0;
bool CEntity::getMovingStatus()
{
    return moving;
}
void CEntity::update(int y, int x)
{
    int totalFrames = 6;   // Animation besteht jeweils aus 2 sprites
    int delayPerFrame = 500;
    int movingDirection = 0;

    if (x > 0)
        movingDirection = 0; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
    if (x < 0)
        movingDirection = 12;// Anfangsprite ist drei weiter Rechts auf dem Spritesheet
    if (y < 0)
        movingDirection = 6;
    if (y > 0)
        movingDirection = 18;

    int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
    textureCoords.x = frame * textureCoords.w;
    textureCoords.y = 64;
    if (y == 0 && x == 0)
    {
        textureCoords.x = 16 * ((SDL_GetTicks() / delayPerFrame) % 4);
        textureCoords.y = 0;
        textureCoords.h = 32;
        textureCoords.w = 16;
    }
}

int CEntity::onInteract()
{
    return 0;
}

void CEntity::renderer(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, &textureCoords, &bounds);
}

CEntity::CEntity(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving)
{
	this->entityID = entityCount;
	entityCount++;
	texture = textureTemp;
	this->tag = tag;
	this->bounds = bounds;
	this->textureCoords = textureCoords;
    this->walkingDirections = { 0,0 };
    this->moving = moving;
 
}

CEntity::~CEntity()
{

}

CEntity::CEntity()
{

}

structForWalkingDirections* CEntity::getWalkingDirections()
{
    return &walkingDirections;
}

int CEntity::getHealth()
{
	return 1;
}

void CEntity::setHealth(int healthpoints)
{
	return;
}

int CEntity::getMaxHealth()
{
	return 1;
}

void CEntity::setBounds(int y, int x)
{
	bounds.x += x;
	bounds.y += y;
}

SDL_Rect* CEntity::getBounds()
{
	return &bounds;
}

int CEntity::getID()
{
	return entityID;
}



