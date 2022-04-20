#include "CGamemaster.h"
#include "Resources.h"
#include "CEnemy.h"

#include "SDL_ttf.h"

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

    /* Input */

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

        /* Update */

        if (deltaTime < float(1000 / 60)) //Limit FPS auf 60
        {
            SDL_Delay(float(1000 / 60) - deltaTime);
        }

        spielerPointer->animation(y_axis, x_axis, deltaTime);
        spielerPointer->bewegen(y_axis*deltaTime * 0.225, x_axis*deltaTime * 0.225);

        /* Render */

        SDL_RenderCopy(renderer, currentMap->getTexture(), NULL, currentMap->getPosition());
        spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern
        SDL_RenderCopy(renderer, currentMap_TopLayer->getTexture(), NULL, currentMap_TopLayer->getPosition());



        for (auto cursor : listeVonEntitys)
        {
            cursor->update(y_axis, x_axis);
            cursor->renderer(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();
}

void CGamemaster::init()
{

    if (TTF_Init() < 0)
        cout << "Error: " << TTF_GetError() << endl;

    SDL_Surface* tempSurface = IMG_Load(RSC_CHARAKTER_SPRITE);
    CMapEntity* tempMapEntity;
    CEntity* tempEntity;
    SDL_Rect tempBounds;
    SDL_Rect tempTextureCoords;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_Texture* tempTexture;
    tempBounds.x = SCREEN_WIDTH / 2 + 8; //right of the window
    tempBounds.y = SCREEN_HEIGHT / 2; //bottom of the window
    tempBounds.w = 16 * 2;
    tempBounds.h = 32 * 2;

    spielerPointer = new CPlayer(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Player", tempBounds, tempTextureCoords);
    
    tempSurface = IMG_Load(RSC_MAP1_SPRITE);
    tempBounds.x = 0; //Extreme left of the window
    tempBounds.y = 0; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 2;
    tempBounds.h *= 2;
    currentMap = new CMap(tempTexture, tempBounds);

    tempSurface = IMG_Load(RSC_MAP1_SPRITE_TOP_LAYER);
    tempBounds.x = 0; //Extreme left of the window
    tempBounds.y = 0; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 2;
    tempBounds.h *= 2;
    currentMap_TopLayer = new CMap(tempTexture, tempBounds);//Nur die aktuelle Karte wird abgespeichert, damit nicht unötig Speicherplatz verschwendet wird


    
    tempBounds.x = 0;
    tempBounds.y = 0;
    tempBounds.w = 16 * 2;
    tempBounds.h = 175 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die erste Kollisionszone wird erstellt

    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 0;
    tempBounds.y = 160*2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die zweite Kollisionszone wird erstellt

    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 48*2;
    tempBounds.y = 160 * 2;
    tempBounds.w = 96 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die dritte Kollisionszone wird erstellt

    currentMap->addObjectToMap(tempMapEntity);

    tempSurface = IMG_Load(RSC_BANDIT_SPRITE);
    tempBounds.x = 10; //Extreme left of the window
    tempBounds.y = 350; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 16;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 2*tempBounds.w / 12; // Breite geteilt durch anzahl Frames
    tempBounds.h = 2 * tempBounds.h / 4; // Hoehe geteilt durch anzahl der Zeilen von Frames
    tempEntity = new CEnemy(SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, 1, 1, 6, 4, 4, 2);
    listeVonEntitys.push_back(tempEntity);

    spielerPointer->setCurrentMap(currentMap);

    this->gameLoop();
}

int CGamemaster::getWidthOfWindow()
{
    return SCREEN_WIDTH;
}

int CGamemaster::getHeigthOfWindow()
{
    return SCREEN_HEIGHT;
}

void CGamemaster::moveMaps(int x, int y)
{
    SDL_Rect* MapDataPtr = currentMap->getPosition();
    MapDataPtr->x += x;
    MapDataPtr->y += y;

    MapDataPtr = currentMap_TopLayer->getPosition();
    MapDataPtr->x += x;
    MapDataPtr->y += y;
}
