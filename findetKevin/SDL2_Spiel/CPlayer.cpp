#include "CPlayer.h"
#include "CGamemaster.h";
CPlayer::CPlayer(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords) : CEntity(textureTemp, tag, bounds, textureCoords)
{
    this->game = game;
	healItems = 0;
	healthPoints = 20; 
    footSpace.x = game->getWidthOfWindow() / 2 + 8; 
    footSpace.y = game->getHeigthOfWindow() / 2+24*2; 
    footSpace.w = 15 * 2;
    footSpace.h = 8 * 2;
}

void CPlayer::setCurrentMap(CMap *map)
{
    this->currentmap = map;
}

int CPlayer::bewegen(int y, int x)
{
    bool x_collision = true, y_collision = true;
    SDL_Rect* temp;
    footSpace.x += x;   //Zuerst bewege ich den Spieler und schaue ob er gegen etwas stoesst, danach erst bewege ich wirklich alle anderen Elemente
    for (auto cursor : currentmap->getListeVonEntitys())
    {
        if (SDL_HasIntersection(&footSpace, cursor->getBounds()))
        {
            x_collision = false;
        }
    }
    footSpace.x -= x;
    x *= -1; //Wir wollen die Welt um den Spieler bewegen und nicht den Spieler durch die Welt, darum invertieren wir die Richtungsvektoren
    if (x_collision)
    {
        for (auto cursor : currentmap->getListeVonEntitys())
        {
            temp = cursor->getBounds();
            temp->x += x;
        }

    }

    footSpace.y += y;   //Zuerst bewege ich den Spieler und schaue ob er gegen etwas stoesst, danach erst bewege ich wirklich alle anderen Elemente
    for (auto cursor : currentmap->getListeVonEntitys())
    {
        if (SDL_HasIntersection(&footSpace, cursor->getBounds()))
        {
            y_collision = false;
        }
    }
    footSpace.y -= y;
    y *= -1;
    if (y_collision)
    {
        for (auto cursor : currentmap->getListeVonEntitys())
        {
            temp = cursor->getBounds();
            temp->y += y;
        }

    }

    game->moveMaps(x * x_collision, y * y_collision);

    return true;
}

void CPlayer::animation(int y, int x, double deltaTime)
{
    int totalFrames = 6;   // Animation besteht jeweils aus 2 sprites
    int delayPerFrame = 100;
    int movingDirection = 0;

    if (x > 0 )
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
        textureCoords.x = 48;
        textureCoords.y = 0;
        textureCoords.h = 32;
        textureCoords.w = 16;        
    }
}