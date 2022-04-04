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
    window = SDL_CreateWindow("Findet Kevin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    spielerPointer = NULL;
}

void CGamemaster::gameLoop()
{
    int y_axis = 0;
    int x_axis = 0;
    bool quit = false;
    Uint32 currentTime = SDL_GetTicks(); //Zum errechnen der Deltatime
    while (!quit)
    {
        Uint32 lastTime = currentTime;
        currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime);
        std::cout << deltaTime << std::endl;
        while (SDL_PollEvent(&input) > 0)
        {
            switch (input.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYUP:

                switch (input.key.keysym.sym)
                {

                case SDLK_w:
                case SDLK_s:
                    y_axis = 0;
                    break;

                case SDLK_d:
                case SDLK_a:
                    x_axis = 0;
                    break;
                }
                break;

            case SDL_KEYDOWN:

                switch (input.key.keysym.sym)
                {

                case SDLK_w:
                    y_axis = -1;
                    break;
                case SDLK_s:
                    y_axis = 1;
                    break;

                case SDLK_a:
                    x_axis = -1;
                    break;
                case SDLK_d:
                    x_axis = 1;
                    break;
                }

                break;
            }
        }
        if (deltaTime < float(1000 / 60)) //Limit FPS auf 60
        {
            SDL_Delay(float(1000 / 60) - deltaTime);
        }
        spielerPointer->animation(y_axis, x_axis, deltaTime);
        spielerPointer->bewegen(y_axis*deltaTime * 0.225, x_axis*deltaTime * 0.225);
        spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern
        SDL_RenderPresent(renderer);
    }

    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();
}

void CGamemaster::init()
{
    SDL_Surface* tempSurface = IMG_Load(RSC_CHARAKTER_SPRITE);
    SDL_Rect tempBounds;
    SDL_Texture* tempTexture;
    tempBounds.x = 80; //Extreme left of the window
    tempBounds.y = 90; //Very top of the window
    tempBounds.w = 32 * 3;
    tempBounds.h = 32 * 3;
    spielerPointer = new CPlayer(SDL_CreateTextureFromSurface(renderer, tempSurface), "Player", tempBounds);
    
    tempSurface = IMG_Load(RSC_MAP1_SPRITE);
    tempBounds.x = 0; //Extreme left of the window
    tempBounds.y = 0; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 3;
    tempBounds.h *= 3;
    CMap * Map1 = new CMap(tempTexture, tempBounds);
    //Hier wird eine Liste aller Maps benötigt, dort wird diese eingefügt und so kann der GameMaster sie verwalten
    /*listeVonEntitys.push_back( );*/
    SDL_RenderCopy(renderer, tempTexture, NULL, &tempBounds);
    this->gameLoop();
}
