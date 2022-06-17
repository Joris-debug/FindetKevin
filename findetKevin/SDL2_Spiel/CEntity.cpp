#include "CEntity.h"
#include "CGamemaster.h"
int CEntity::entityCount = 0;
bool CEntity::getMovingStatus()
{
    return moving;
}
void CEntity::update(int y, int x)
{

}

int CEntity::onInteract()
{
    return 3;
}

void CEntity::renderer(SDL_Renderer* renderer)
{

    if (textureCoords.w * 2 != bounds.w && textureCoords.h * 2 != bounds.h) //Entity befindet sich in der Mitte des Sprites
    {
        SDL_Rect tempRect;
        tempRect.w = textureCoords.w * 2;
        tempRect.h = textureCoords.h * 2;
        tempRect.x = bounds.x - bounds.w / 2;
        tempRect.y = bounds.y - bounds.h / 2;
        SDL_RenderCopy(renderer, texture, &textureCoords, &tempRect);
    }
    else
    SDL_RenderCopy(renderer, texture, &textureCoords, &bounds);
}

CEntity::CEntity(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving)
{
	this->entityID = entityCount;
	entityCount++;
	texture = textureTemp;
	this->tag = tag;
	this->bounds = bounds;
	this->textureCoords = textureCoords;
    this->walkingDirections = { 0,0 };
    this->moving = moving;
    this->game = game;

    cout << "Entity " << tag << ", ist gespawned" << endl;
}

CEntity::~CEntity()
{
    SDL_DestroyTexture(texture);
    cout << "Entity " << tag << " ist eliminiert" << endl;
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

void CEntity::entityPathfinding(double deltaTime)
{
    if (!moving)
        return;     
    int walkingDirectionX = walkingDirections.xDirection;  //Ich hole mir die Aktuelle Laufrichtung
    int walkingDirectionY = walkingDirections.yDirection;
    srand(SDL_GetTicks() * entityID);
    if (SDL_GetTicks() % 2000 <= 100)         //Alle 2000 Ticks wird ein check gemacht ob die Richtung geändert wird(außerdem runde ich da nicht alle Computer gleich performen)
    {
        if (rand() % 5 == 1)   //Nur in 20% der Fällen wird wirklich die richtung verändert
        {
            walkingDirectionX = (rand() % 3) - 1;
            walkingDirectionY = (rand() % 3) - 1;
            walkingDirections.xDirection = walkingDirectionX;
            walkingDirections.yDirection = walkingDirectionY;
        }
        else if (walkingDirectionX == 0 && walkingDirectionY == 0)      //In dem Fall muss nicht der Rest der Methode durchlaufen werden
        {
            update(walkingDirectionY, walkingDirectionX);  //Neuer Sprite wird geladen
            return;
        }
    }
    int x = walkingDirectionX * deltaTime;
    int y = walkingDirectionY * deltaTime;
    bool x_collision = true, y_collision = true;
    setBounds(0, x);

    for (auto cursorMapEntity : game->getMap()->getListeVonEntitys())
    {
        if (SDL_HasIntersection(&bounds, cursorMapEntity->getBounds()))
        {
            x_collision = false;
        }
    }

    for (auto cursor : *game->getlisteVonEntitys())          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
    {
        if (SDL_HasIntersection(&bounds, cursor->getBounds()) && cursor->getID() != entityID)  //Der Gegner soll nicht in andere Gegner laufen, aber er selbst befindet sich auch in der Liste, das muss abgefangen werden
        {
            x_collision = false;
        }

    }

    if (!x_collision)
    {
        setBounds(0, -x);
        if (rand() % 4 == 3)
            walkingDirections.xDirection = 0;
        else
            walkingDirections.xDirection = walkingDirections.xDirection * (-1); //Nachdem er gegen eine Wand läuft soll er umkehren oder stehen bleiben, das macht den Gegner dynamischer

    }

    setBounds(y, 0);
    for (auto cursorMapEntity : game->getMap()->getListeVonEntitys())
    {
        if (SDL_HasIntersection(&bounds, cursorMapEntity->getBounds()))
        {

            y_collision = false;
        }
    }

    for (auto cursor : *game->getlisteVonEntitys())          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
    {
        if (SDL_HasIntersection(&bounds, cursor->getBounds()) && cursor->getID() != entityID)
        {
            y_collision = false;
        }

    }

    if (!y_collision)
    {
        setBounds(-y, 0);
        if (rand() % 4 == 2)
            walkingDirections.yDirection = 0;
        else
            walkingDirections.yDirection = walkingDirections.yDirection * (-1); //Nachdem er gegen eine Wand läuft soll er umkehren oder stehen bleiben, das macht den Gegner dynamischer
    }
    update(walkingDirectionY, walkingDirectionX);  //Neuer Sprite wird geladen
    return;
}




