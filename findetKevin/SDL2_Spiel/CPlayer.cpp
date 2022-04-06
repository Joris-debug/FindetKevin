#include "CPlayer.h"
CPlayer::CPlayer(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords) : CEntity(textureTemp, tag, bounds, textureCoords)
{
	healItems = 0;
	healthPoints = 20; 
    footSpace.x = 50; //Extreme left of the window
    footSpace.y = 90; //Very top of the window
    footSpace.w = 16 * 2;
    footSpace.h = 8 * 2;
}

void CPlayer::setCurrentMap(CMap &map)
{
    this->currentmap = &map;
}

int CPlayer::bewegen(int y, int x)
{


    footSpace.x += x;
    footSpace.y += y;
    for (auto cursor : currentmap->getListeVonEntitys())
    {
        SDL_Rect temp = cursor->getBounds();
        if (SDL_HasIntersection(&footSpace, &temp))
        {
            footSpace.x -= x;
            footSpace.y -= y;
            return false;
        }
    }

    bounds.x = bounds.x + x;
    bounds.y = bounds.y + y;
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