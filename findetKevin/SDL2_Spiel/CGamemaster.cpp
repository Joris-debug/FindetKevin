#include "CGamemaster.h"

#include "Resources.h"

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

void CGamemaster::gameLoop()
{
    bool quit = false;
    Uint32 currentTime = SDL_GetTicks(); //Zum errechnen der Deltatime
    while (!quit)
    {
        Uint32 lastTime = currentTime;
        currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 3.5f;
        std::cout << deltaTime << std::endl;
        while (SDL_PollEvent(&input) > 0)
        {

            switch (input.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }
        if (deltaTime < float(1000 / 60)) //Limit FPS auf 60
        {
            SDL_Delay(float(1000 / 60) - deltaTime);
        }

        spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern
        SDL_RenderPresent(renderer);
    }

    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();
}

void CGamemaster::init()
{
    SDL_Surface* tempSurface = IMG_Load(RSC_CHARACKTER_SPRITE);
    SDL_Rect tempBounds;
    tempBounds.x = 0; //Extreme left of the window
    tempBounds.y = 0; //Very top of the window
    tempBounds.w = 32 * 4;
    tempBounds.h = 32 * 4;
    CEntity* ptr = new CPlayer(SDL_CreateTextureFromSurface(renderer, tempSurface), "Player", tempBounds);
    spielerPointer = ptr;
    listeVonEntitys.push_back(ptr);

    this->gameLoop();
}
