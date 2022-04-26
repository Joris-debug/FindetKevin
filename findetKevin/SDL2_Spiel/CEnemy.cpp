#include "CEnemy.h"

CEnemy::CEnemy(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, int healthpoints, int damage, int walkingFrames, int attackFrames, int idleFrames, int deathFrames) : CEntity(textureTemp, tag, bounds, textureCoords)
{
    this->healthpoints = healthpoints;
    this->maxHealthpoints = healthpoints;
    this->damage = damage;
    this->walkingFrames = walkingFrames;
    this->attackFrames = attackFrames;
    this->idleFrames = idleFrames;
    this->deathFrames = deathFrames;
    this->walkingDirections = { 0,0 };
}

void CEnemy::onInteract()
{
    healthpoints--;

}

int CEnemy::getHealth()
{
    return healthpoints;
}

void CEnemy::setHealth(int healthpoints)
{
    this->healthpoints = healthpoints;
}

int CEnemy::getMaxHealth()
{
    return maxHealthpoints;
}


void CEnemy::update(int y, int x)
{

    int totalFrames = 6;   // Animation besteht jeweils aus 6 sprites
    int delayPerFrame = 100;
    int movingDirection = 0;
    if (x > 0)
        movingDirection = 0; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
    else
        movingDirection = 6;// Anfangsprite ist drei weiter Rechts auf dem Spritesheet

    if (maxHealthpoints > healthpoints)
    {
        totalFrames = 2;
        if (x <= 0)
            movingDirection = 2;// Anfangsprite ist drei weiter Rechts auf dem Spritesheet
    }

    int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
    textureCoords.x = frame * textureCoords.w;
    textureCoords.y = 16;

    if (healthpoints == 0) //Siehe Kommentar in Zeile 60
    {
        return;         //wenn der Spieler jetzt noch im Gegner steht wird ersterer terminiert
    }

    if (maxHealthpoints > healthpoints)  //Wenn man stirbt muss nicht die idle animation abgespielt werden
    {
        textureCoords.y = 48;
        return;
    }

    if (y == 0 && x == 0)
    {
        textureCoords.x = 0;
        textureCoords.y = 0;
        textureCoords.h = 16;
        textureCoords.w = 16;
    }



}

structForWalkingDirections* CEnemy::getWalkingDirections()
{
    return &walkingDirections;
}
