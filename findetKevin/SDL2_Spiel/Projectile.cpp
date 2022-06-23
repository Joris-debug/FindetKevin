#include "Projectile.h"

#include "Resources.h"

Projectile::Projectile(CGamemaster* game, Direction dir, int startX, int startY, float speed, float maxDist)
{
	SDL_Surface* tempSurface = IMG_Load(RSC_PROJECTILE_SPRITE);
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(game->getRenderer(), tempSurface);

	this->textureCoords.x = 5;
	this->textureCoords.y = ((dir + 1) * 16) + 5;
	this->textureCoords.w = 6;
	this->textureCoords.h = 5;

	this->texture = tempTexture;

	this->maxTravelDist = maxDist;

	this->startPos.x = startX;
	this->startPos.y = startY;

	this->bounds.x = startX;
	this->bounds.x = startY;
	this->bounds.w = 10;
	this->bounds.h = 10;

	this->isActive = true;

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

	//game->getlisteVonEntitys()->push_back(this);
	game->getAllProjectiles()->emplace(this);
}

void Projectile::update(int y, int x)
{
	if (!isActive)
		return;
	this->speed.addTo(&(this->bounds));

	vec2 pos(bounds.x, bounds.y);

	lifeTime++;

	if (lifeTime > maxTravelDist)
	{
		std::cout << "Projectile deactivated" << std::endl;
		isActive = false;
		game->getAllProjectiles()->erase(this);
		delete this;
	}
}

void Projectile::render(SDL_Renderer* renderer)
{

}

int Projectile::onInteract()
{
	return 3;
}