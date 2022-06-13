#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
#undef main

int main()
{
    std::locale::global(std::locale(""));   //Um Umlaute zu sehen
    CGamemaster GameMasterClass;
    GameMasterClass.titlescreen(); 
    return 0;
}

