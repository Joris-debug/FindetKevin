#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
#undef main

CGamemaster GameMasterClass;
int main()
{
    GameMasterClass.GameLoop();
    return 0;
}

