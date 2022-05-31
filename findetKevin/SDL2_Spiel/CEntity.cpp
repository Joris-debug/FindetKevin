#include "CEntity.h"
#include "CGamemaster.h"
int CEntity::entityCount = 0;
bool CEntity::getMovingStatus()
{
    return moving;
}
void CEntity::update(int y, int x)
{
    int totalFrames = 6;   // Animation besteht jeweils aus 2 sprites
    int delayPerFrame = 500;
    int movingDirection = 0;

    if (x > 0)
        movingDirection = 0; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
    if (x < 0)
        movingDirection = 12;// Anfangsprite ist drei weiter Rechts auf dem Spritesheet
    if (y < 0)
        movingDirection = 6;
    if (y > 0)
        movingDirection = 18;

    int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
    textureCoords.x = frame * textureCoords.w;
    textureCoords.y = 64;
    if (y == 0 && x == 0)
    {
        textureCoords.x = 16 * ((SDL_GetTicks() / delayPerFrame) % 4);
        textureCoords.y = 0;
        textureCoords.h = 32;
        textureCoords.w = 16;
    }
}

int CEntity::onInteract()
{
    return 3;
}

void CEntity::renderer(SDL_Renderer* renderer)
{
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

}

CEntity::~CEntity()
{

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

    for (auto cursor : game->getlisteVonEntitys())          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
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

    for (auto cursor : game->getlisteVonEntitys())          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
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




