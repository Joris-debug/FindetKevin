#include "CGamemaster.h"

CGamemaster::CGamemaster()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    //For loading PNG images
    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        // handle error
    }
    window = SDL_CreateWindow("Findet Kevin", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void CGamemaster::GameLoop()
{
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&input) > 0)
        {

            switch (input.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }

    }



    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();
}