#include "CEnemy.h"

CEnemy::CEnemy(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving, int healthpoints, int damage, int walkingFrames, int attackFrames, int idleFrames, int deathFrames) : CEntity(game, textureTemp, tag, bounds, textureCoords, moving)
{
    this->healthpoints = healthpoints;
    this->maxHealthpoints = healthpoints;
    this->damage = damage;
    this->walkingFrames = walkingFrames;
    this->attackFrames = attackFrames;
    this->idleFrames = idleFrames;
    this->deathFrames = deathFrames;

    this->cooldown = -1;
    this->framesAfterLastAttack = 0;
}

int CEnemy::onInteract()
{
    healthpoints--;
    return 0;
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
    int startSpriteIdle = 0;
    if (walkingDirections.xDirection == -1)         //Warum?? weil ich bei statischen Gegnern in diesem struct die ausrichtung abspeichern will
        startSpriteIdle = idleFrames / 2;
    if (y == 0 && x == 0)
    {
        textureCoords.x = 16 * ((SDL_GetTicks() / delayPerFrame) % idleFrames/2) + startSpriteIdle;
        textureCoords.y = 0;
        textureCoords.h = 16;
        textureCoords.w = 16;
    }

    framesAfterLastAttack++;
    if (cooldown < 0)
        return;
    if (framesAfterLastAttack >= cooldown)
    {
        shouldShoot();
    }
}

void CEnemy::shouldShoot()
{
    SDL_Rect* playerBounds = this->game->getPlayer()->getBounds();
    vec2 playerCenter(playerBounds->x + (playerBounds->w / 2), playerBounds->y + (playerBounds->h / 2));
    vec2 ownCenter(bounds.x + (bounds.w / 2), bounds.y + (bounds.h / 2));

    float xDist = ownCenter.getXDist(playerCenter);
    float yDist = ownCenter.getYDist(playerCenter);
    float dist = ownCenter.getDist(playerCenter);

    float agroWidth = 40.0f;
    float agroRadius = 200;
    float passRadius = 60;

    if (xDist < agroWidth && dist < agroRadius && dist > passRadius)
    {
        std::cout << "Player in Raduis" << std::endl;
        if (ownCenter.y < playerCenter.y)    // Over the player
        {
            //std::cout << "Shooting down" << std::endl;
            game->getlisteVonEntitys()->push_back(new Projectile(game, DOWN, ownCenter.x, ownCenter.y, 5.0f, 300.0f));
            framesAfterLastAttack = 0;
        }
        else if (ownCenter.y > playerCenter.y)    // Under the player
        {
            //std::cout << "Shooting up" << std::endl;
            game->getlisteVonEntitys()->push_back(new Projectile(game, UP, ownCenter.x, ownCenter.y, 5.0f, 300.0f));
            framesAfterLastAttack = 0;
        }
    }else if(yDist < agroWidth && dist < agroRadius && dist > passRadius)
    {
        if (ownCenter.x < playerCenter.x)    // Left of the player
        {
            //std::cout << "Shooting right" << std::endl;
            game->getlisteVonEntitys()->push_back(new Projectile(game, RIGHT, ownCenter.x, ownCenter.y, 5.0f, 300.0f));
            framesAfterLastAttack = 0;
        }
        else if (ownCenter.x > playerCenter.x)    // Right of the player
        {
            //std::cout << "Shooting left" << std::endl;
            game->getlisteVonEntitys()->push_back(new Projectile(game, LEFT, ownCenter.x, ownCenter.y, 5.0f, 300.0f));
            framesAfterLastAttack = 0;
        }
    }

    return;
}

