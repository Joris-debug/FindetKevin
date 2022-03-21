#include "CEntity.h"
void CEntity::update()
{

}
void CEntity::renderer(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, &textureCoords, &bounds);

}

CEntity::CEntity(SDL_Texture* textureTemp, string tag, SDL_Rect bounds)
{
	texture = textureTemp;
	this->tag = tag;
	this->bounds = bounds;
	textureCoords.x = 0;
	textureCoords.y = 0;
	textureCoords.h = 32;
	textureCoords.w = 32;
}

CEntity::~CEntity()
{


}
CEntity::CEntity()
{


}