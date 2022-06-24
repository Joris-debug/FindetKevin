#include "Projectile.h"

#include "Resources.h"

Projectile::Projectile(CGamemaster* game, Direction dir, int startX, int startY, float speed, float maxDist)
{
	SDL_Surface* tempSurface = IMG_Load(RSC_PROJECTILE_SPRITE);
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(game->getRenderer(), tempSurface);

	//std::cout << dir << std::endl;

	/*
	this->textureCoords.x = 5;
	this->textureCoords.y = ((dir + 1) * 16) + 5;
	this->textureCoords.w = 6;
	this->textureCoords.h = 5;
	*/

	this->texture = tempTexture;
	this->game = game;

	this->maxTravelDist = maxDist;

	this->startPos.x = startX;
	this->startPos.y = startY;

	this->killFlag = false;
	this->tag = "projectile";

	this->isActive = true;

	if (dir == RIGHT)
	{
		//this->speed.x = speed;
		this->textureCoords.x = 6;
		this->textureCoords.y = 7;
		this->textureCoords.w = 5;
		this->textureCoords.h = 3;
		this->walkingDirections.xDirection = speed;
	}
	else if (dir == LEFT)
	{
		//this->speed.x = -speed;
		this->textureCoords.x = 5;
		this->textureCoords.y = 23;
		this->textureCoords.w = 5;
		this->textureCoords.h = 3;
		this->walkingDirections.xDirection = -speed;
	}
	else if (dir == UP)
	{
		//this->speed.y = -speed;
		this->textureCoords.x = 7;
		this->textureCoords.y = 37;
		this->textureCoords.w = 3;
		this->textureCoords.h = 5;
		this->walkingDirections.yDirection = speed * (-1);
		this->walkingDirections.xDirection = 0;
	}
	else if (dir == DOWN)
	{
		//this->speed.y = speed;
		this->textureCoords.x = 7;
		this->textureCoords.y = 54;
		this->textureCoords.w = 3;
		this->textureCoords.h = 5;
		this->walkingDirections.yDirection = speed;
	}

	this->bounds.x = startX;
	this->bounds.y = startY;
	this->bounds.w = textureCoords.w * 2;
	this->bounds.h = textureCoords.h * 2;

	//std::cout << "walking dir " << walkingDirections.xDirection << " " << walkingDirections.yDirection << std::endl;
	game->getlisteVonEntitys()->push_back(this);

	/*
	int tempY = walkingDirections.yDirection;
	walkingDirections.yDirection = walkingDirections.xDirection;
	walkingDirections.xDirection = tempY;
	*/

	//game->getAllProjectiles()->emplace(this);
}

void Projectile::entityPathfinding(double dt)
{
	//std::cout << "Pathfinding entity" << std::endl;

	setBounds(walkingDirections.yDirection * dt, walkingDirections.xDirection * dt);

	bool willHaveToBeDeleted = false;

	for (auto cursorMapEntity : game->getMap()->getListeVonEntitys())	// Mapentitys
	{
		if (SDL_HasIntersection(&bounds, cursorMapEntity->getBounds()))
		{
			//x_collision = false;
			willHaveToBeDeleted = true;
			//std::cout << "collision detected!" << std::endl;
			break;
		}
	}

	for (auto cursor : *game->getlisteVonEntitys())          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
	{
		if (SDL_HasIntersection(&bounds, cursor->getBounds()) && cursor->getID() != entityID)  //Der Gegner soll nicht in andere Gegner laufen, aber er selbst befindet sich auch in der Liste, das muss abgefangen werden
		{
			//x_collision = false;
			//willHaveToBeDeleted = true;
			break;
		}
	}

	if (SDL_HasIntersection(game->getPlayer()->getBounds(), &bounds))
	{
		game->getPlayer()->takeDmg(1);
		willHaveToBeDeleted = true;
	}

	if (willHaveToBeDeleted)
	{
		//game->getlisteVonEntitys()->remove(this);             //Münze Löschen
		this->killFlag = true;
		//game->getlisteVonEntitys()->erase(this);
	}
	//update(walkingDirectionY, walkingDirectionX);  //Neuer Sprite wird geladen
}

void Projectile::update(int y, int x)
{
	/*
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
		//game->getAllProjectiles()->erase(this);
		//free(this);
	}
	*/
}

/*
void Projectile::render(SDL_Renderer* renderer)
{

}
*/
int Projectile::onInteract()
{
	return 3;
}