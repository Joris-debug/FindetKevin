#pragma once
#include "CMapEntity.h"
class CMap
{
private:
	SDL_Rect position;
	SDL_Texture* texture;
	list <CMapEntity*> listeVonMapEntitys;
public:
	CMap(SDL_Texture* textureTemp, SDL_Rect coords);
	SDL_Texture* getTexture();
	SDL_Rect* getPosition();
	void addObjectToMap(CMapEntity * kollisionszone);
	list <CMapEntity*> getListeVonEntitys();
	void setListeVonEntitys(list <CMapEntity*> listeVonEntitys);
};

