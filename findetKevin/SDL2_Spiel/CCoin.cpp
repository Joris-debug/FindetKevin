#include "CCoin.h"
#include "Resources.h"
#include "CGamemaster.h"
CCoin::CCoin(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CEntity(game, textureTemp, tag, bounds, textureCoords, false)
{	
	SDL_Surface* tempSurface = IMG_Load(RSC_COIN_SPRITE);	//Jede Coin hat die selbe Textur
	this->texture = SDL_CreateTextureFromSurface(game->getRenderer(), tempSurface);
	SDL_FreeSurface(tempSurface);
	this->bounds.w = 32;
	this->bounds.h = 32;
	this->textureCoords.x = 0;
	this->textureCoords.y = 0;
	this->textureCoords.w = 16;
	this->textureCoords.h = 16;
}
int CCoin::onInteract()
{
	game->getPlayer()->addPoints(100);
	game->getlisteVonEntitys()->remove(this);             //Münze Löschen
	delete this;
	return 4;
}
void CCoin::update(int y, int x)
{
	textureCoords.x = 0 + 16 * ((SDL_GetTicks() / 100) % 9);
	textureCoords.y = 0;
	return;
}
