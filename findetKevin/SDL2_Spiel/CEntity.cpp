#include "CEntity.h"
int CEntity::entityCount = 0;
void CEntity::update(int y, int x)
{
}

void CEntity::onInteract()
{
}

void CEntity::renderer(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, &textureCoords, &bounds);
}

CEntity::CEntity(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords)
{
	this->entityID = entityCount;
	entityCount++;
	texture = textureTemp;
	this->tag = tag;
	this->bounds = bounds;
	this->textureCoords = textureCoords;
}

CEntity::~CEntity()
{

}

CEntity::CEntity()
{

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

