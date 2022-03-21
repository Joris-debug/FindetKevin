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