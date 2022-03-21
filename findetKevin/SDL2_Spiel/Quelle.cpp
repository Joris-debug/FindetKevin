#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
#undef main

int main()
{
    CGamemaster GameMasterClass;
    GameMasterClass.init();
    return 0;
}

