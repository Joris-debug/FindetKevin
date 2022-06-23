#include "CMap.h"
CMap::~CMap()
{
	SDL_DestroyTexture(texture);
}

CMap::CMap(SDL_Texture* textureTemp, SDL_Rect coords)
{
	position = coords;
	texture = textureTemp;
}

SDL_Texture* CMap::getTexture()
{
	return this->texture;
}

SDL_Rect* CMap::getPosition()
{
	return &this->position;
}

void CMap::addObjectToMap(CMapEntity * kollisionszone)
{
	listeVonMapEntitys.push_back(kollisionszone);
}

list<CMapEntity*> CMap::getListeVonEntitys()
{
	return listeVonMapEntitys;
}

void CMap::setListeVonEntitys(list<CMapEntity*> listeVonEntitys)
{
	this->listeVonMapEntitys = listeVonEntitys;
}
