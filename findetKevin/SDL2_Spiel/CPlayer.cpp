#include "CPlayer.h"
CPlayer::CPlayer(SDL_Texture* textureTemp, string tag, SDL_Rect bounds) : CEntity(textureTemp, tag, bounds)
{
	healItems = 0;
	healthPoints = 20; 
    footSpace.x = 0; //Extreme left of the window
    footSpace.y = 0; //Very top of the window
    footSpace.w = 32 * 4;
    footSpace.h = 8 * 4;
}

int CPlayer::bewegen(int y, int x)
{
    bounds.x = bounds.x + x;
    bounds.y = bounds.y + y;

    return true;
}

void CPlayer::animation(int y, int x, double deltaTime)
{
    int totalFrames = 2;   // Animation besteht jeweils aus 2 sprites
    int delayPerFrame = 1000;
    int movingDirection;
    if (x < 0 )
        movingDirection = 1; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
    else
        movingDirection = 3;// Anfangsprite ist drei weiter Rechts auf dem Spritesheet
    int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
    textureCoords.x = frame * textureCoords.w;
    if (y == 0 && x == 0)
    {       
        textureCoords.x = 0;
        textureCoords.y = 0;
        textureCoords.h = 32;
        textureCoords.w = 32;        
    }
}