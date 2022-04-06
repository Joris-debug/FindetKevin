#include "CEntity.h"
void CEntity::update()
{

}
void CEntity::renderer(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, &textureCoords, &bounds);
}

CEntity::CEntity(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords)
{
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