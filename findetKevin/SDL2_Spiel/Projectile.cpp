#include "Projectile.h"

#include "Resources.h"

Projectile::Projectile(CGamemaster* game, Direction dir, int startX, int startY, float speed)
{
	SDL_Surface* tempSurface = IMG_Load(RSC_PROJECTILE_SPRITE);
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(game->getRenderer(), tempSurface);

	this->textureCoords.x = 5;
	this->textureCoords.y = ((dir + 1) * 16) + 5;
	this->textureCoords.w = 6;
	this->textureCoords.h = 5;

	this->texture = tempTexture;

	this->tag = "projectile";

	this->bounds.x = startX;
	this->bounds.x = startY;
	this->bounds.w = 10;
	this->bounds.h = 10;

	this->moving = true;

	switch (dir)
	{
	case RIGHT:
		this->speed.x = speed;
		break;
	case LEFT:
		this->speed.x = -speed;
		break;
	case UP:
		this->speed.y = -speed;
		break;
	case DOWN:
		this->speed.y = speed;
		break;
	default:
		break;
	}

	game->getlisteVonEntitys()->push_back(this);
}

void Projectile::update(int y, int x)
{
	this->speed.addTo(&(this->bounds));
}

int Projectile::onInteract()
{
	return 3;
}