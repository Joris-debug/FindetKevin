#include "CGamemaster.h"
#include "Resources.h"
#include "CEnemy.h"
#include <SDL_ttf.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

CGamemaster::CGamemaster()
{
    if (TTF_Init() < 0) {
        cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
    }//Um Text zu rendern
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

CGamemaster::~CGamemaster()
{
    cout << endl << "GameMasterclass wurde terminiert" << endl;
    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    SDL_Quit();
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

        for (auto cursor : listeVonEntitys)
            cursor->update(0, 0);
        spielerPointer->animation(y_axis, x_axis, deltaTime);
        collisionDetection(spielerPointer->bewegen(y_axis * deltaTime * 0.225, x_axis * deltaTime * 0.225));
        SDL_RenderCopy(renderer, currentMap->getTexture(), NULL, currentMap->getPosition());
        spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern
        
        enemyPathfinding(deltaTime * 0.1);
        for (auto cursor : listeVonEntitys)
        {
            cursor->renderer(renderer);
        }
        SDL_RenderCopy(renderer, currentMap_TopLayer->getTexture(), NULL, currentMap_TopLayer->getPosition());
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

    }


}

void CGamemaster::init()
{
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
    tempBounds.x = -208*2; //Extreme left of the window
    tempBounds.y = 0; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 2;
    tempBounds.h *= 2;
    currentMap = new CMap(tempTexture, tempBounds);

    tempSurface = IMG_Load(RSC_MAP1_SPRITE_TOP_LAYER);
    tempBounds.x = -208*2; //Extreme left of the window
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
    tempBounds.y = 160 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die zweite Kollisionszone wird erstellt

    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 48 * 2;
    tempBounds.y = 160 * 2;
    tempBounds.w = 96 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die dritte Kollisionszone wird erstellt

    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 16 * 2;
    tempBounds.y = 16 * 2;
    tempBounds.w = 176 * 2;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die vierte Kollisionszone wird erstellt

    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 192 * 2;
    tempBounds.y = 0;
    tempBounds.w = 16 * 2;
    tempBounds.h = 176 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //usw
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 160 * 2;
    tempBounds.y = 160 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds);
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 175 * 2;
    tempBounds.y = 96 * 2;
    tempBounds.w = 16 * 2;
    tempBounds.h = 64 * 2;
    tempMapEntity = new CMapEntity(tempBounds); // Schrank unten Rechts und Stück der Wand
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 48 * 2;
    tempBounds.y = 64 * 2;
    tempBounds.w = 80 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Obere Tischreihe
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 64 * 2;
    tempBounds.y = 112 * 2;
    tempBounds.w = 80 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Untere Tischreihe
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = 32 * 2;
    tempBounds.y = 96 * 2;
    tempBounds.w = 16 * 2;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Lehrerpult
    currentMap->addObjectToMap(tempMapEntity);

    tempSurface = IMG_Load(RSC_BANDIT_SPRITE);
    tempBounds.x = 50; //Extreme left of the window
    tempBounds.y = 100; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 16;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 2 * tempBounds.w / 12; // Breite geteilt durch anzahl Frames
    tempBounds.h = 2 * tempBounds.h / 4; // Hoehe geteilt durch anzahl der Zeilen von Frames
    tempEntity = new CEnemy(SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, 150, 1, 6, 4, 4, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);

    tempSurface = IMG_Load(RSC_NPC_AMELIA_SPRITE);
    tempBounds.x = 100; //Extreme left of the window
    tempBounds.y = 50; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 16 * 2;
    tempBounds.h = 32 * 2;
    tempEntity = new CEntity(SDL_CreateTextureFromSurface(renderer, tempSurface), "Schuelerin", tempBounds, tempTextureCoords);
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

void CGamemaster::moveEntitys(int x, int y)
{
    for (auto cursor : listeVonEntitys)
    {
        cursor->setBounds(y, x);
    }
}

list<CEntity*> CGamemaster::getlisteVonEntitys()
{
    return listeVonEntitys;
}

int CGamemaster::collisionDetection(int collisionID)
{
    if (collisionID != 2147483647)
    {
        for (auto cursor : listeVonEntitys)
        {
            if (cursor->getID() == collisionID)
            {
                cursor->onInteract();
                if (cursor->getHealth() <= 0) //Dying ist da um die sterbe-Animation abzuspielen, während der animation ist die variable auf true gesetzt
                {
                    listeVonEntitys.remove(cursor);//Gegner getroffen, ZEIT IHN ZU VERNICHTEN MUHAHAHAHAHA (Capslock war an, ups)
                    listeVonEnemies.remove(cursor);
                    delete cursor;
                }
                break;
            }
        }
    }
    return 0;
}

void CGamemaster::enemyPathfinding(double deltaTime)
{

    for (CEntity* cursorEnemy : listeVonEnemies)
    {
        structForWalkingDirections* walkingDirectionPtr = cursorEnemy->getWalkingDirections();     //Ich hole mir die Aktuelle Laufrichtung
        int walkingDirectionX = walkingDirectionPtr->xDirection;
        int walkingDirectionY = walkingDirectionPtr->yDirection;

        srand(time(NULL));
        if (SDL_GetTicks() % 2000 <= 10)         //Alle 2000 Ticks wird ein check gemacht ob die Richtung geändert wird(außerdem runde ich da nicht alle Computer gleich performen)
        {
            if (rand() % 5 == 1)   //Nur in 20% der Fällen wird wirklich die richtung verändert
            {
                walkingDirectionX = (rand() % 3) - 1;
                walkingDirectionY = (rand() % 3) - 1;
                walkingDirectionPtr->xDirection = walkingDirectionX;
                walkingDirectionPtr->yDirection = walkingDirectionY;
            }
            else if (walkingDirectionX == 0 && walkingDirectionY == 0)
                cursorEnemy->update(walkingDirectionY, walkingDirectionX);  //Neuer Sprite wird geladen
            return;
        }
        int x = walkingDirectionX * deltaTime;
        int y = walkingDirectionY * deltaTime;
        bool x_collision = true, y_collision = true;
        SDL_Rect* temp;
        cursorEnemy->setBounds(0, x);
        for (auto cursorMapEntity : currentMap->getListeVonEntitys())
        {
            if (SDL_HasIntersection(cursorEnemy->getBounds(), cursorMapEntity->getBounds()))
            {
                x_collision = false;
            }
        }

        for (auto cursor : listeVonEntitys)          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
        {
            if (SDL_HasIntersection(cursorEnemy->getBounds(), cursor->getBounds()) && cursor->getID() != cursorEnemy->getID())  //Der Gegner soll nicht in andere Gegner laufen, aber er selbst befindet sich auch in der Liste, das muss abgefangen werden
            {
                x_collision = false;
            }

        }

        if (!x_collision)
        {
            cursorEnemy->setBounds(0, -x);
            if (rand() % 4 == 3)
                walkingDirectionPtr->xDirection = 0;
            else
                walkingDirectionPtr->xDirection = walkingDirectionPtr->xDirection * (-1); //Nachdem er gegen eine Wand läuft soll er umkehren oder stehen bleiben, das macht den Gegner dynamischer
                
        }

        cursorEnemy->setBounds(y, 0);
        for (auto cursorMapEntity : currentMap->getListeVonEntitys())
        {
            if (SDL_HasIntersection(cursorEnemy->getBounds(), cursorMapEntity->getBounds()))
            {
                y_collision = false;
            }
        }

        for (auto cursor : listeVonEntitys)          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
        {
            if (SDL_HasIntersection(cursorEnemy->getBounds(), cursor->getBounds()) && cursor->getID() != cursorEnemy->getID())
            {
                y_collision = false;
            }

        }

        if (!y_collision)
        {
            cursorEnemy->setBounds(-y, 0);
            if (rand() % 4 == 2)
                walkingDirectionPtr->yDirection = 0;
            else
                walkingDirectionPtr->yDirection = walkingDirectionPtr->yDirection * (-1); //Nachdem er gegen eine Wand läuft soll er umkehren oder stehen bleiben, das macht den Gegner dynamischer
           

        }


        cursorEnemy->update(walkingDirectionY, walkingDirectionX);  //Neuer Sprite wird geladen

        return;
    }
}

void CGamemaster::titlescreen()
{
    SDL_RenderPresent(renderer);
    TTF_Font* font;

    font = TTF_OpenFont(RSC_FONT_ARIAL, 50);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
    }

    SDL_Surface* text;
    // Set color to white
    SDL_Color color = { 255, 255, 255 };

    text = TTF_RenderText_Solid(font, "Findet Kevin", color);
    if (!text) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    SDL_Texture* text_texture;
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect dest = { SCREEN_WIDTH/2- text->w*0.5,  SCREEN_HEIGHT/2-120,  text->w, text->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);

    font = TTF_OpenFont(RSC_FONT_ARIAL, 24);
    text = TTF_RenderText_Solid(font, "New Game", color);
    if (!text) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    text_texture;
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect startGame = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 30,  text->w, text->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &startGame);

    text = TTF_RenderText_Solid(font, "Select Savefile", color);
    if (!text) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    text_texture;
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2,  text->w, text->h };

    SDL_RenderCopy(renderer, text_texture, NULL, &dest);

    text = TTF_RenderText_Solid(font, "Close Game", color);
    if (!text) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    text_texture;
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect closeButton = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 + 30,  text->w, text->h };

    SDL_RenderCopy(renderer, text_texture, NULL, &closeButton);
    SDL_RenderPresent(renderer);

    SDL_Rect cursor_Hitbox;
    SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);
    cursor_Hitbox.w = 8;
    cursor_Hitbox.h = 8;

    SDL_Event e;
    while (SDL_PollEvent(&e) >= 0)
    {
        SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (SDL_HasIntersection(&cursor_Hitbox, &closeButton))
                return;
            if (SDL_HasIntersection(&cursor_Hitbox, &startGame))
            {
                init();
                return;
            }
        }
        else if (e.type == SDL_QUIT)
            return;
        SDL_Delay(100);
    }
    
}

