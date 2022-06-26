#include "CGamemaster.h"
#include "Resources.h"
#include "CEnemy.h"
#include "CJohn.h"
#include <SDL_ttf.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>      /* time */
#include "CCoin.h"
#include <string>
#include <iomanip> 
#include "IKGameLogic.h"
#include "CQuestTrigger.h"
#include "CNPC.h"
#include "CBuch.h"
#include "CKevin.h"
CGamemaster::CGamemaster()
{
    deltaTime = 1;
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
    spielerPointer = nullptr;
    currentSaveFile = nullptr;
    alleSaveFiles = CSavefile::EinlesenDerSpeicherdaten();
}

CGamemaster::~CGamemaster()
{
    cout << endl << "GameMasterclass wurde terminiert" << endl;
    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

int CGamemaster::gameLoop()
{
    SDL_Event input;
    int y_axis = 0;
    int x_axis = 0;
    Uint32 currentTime = SDL_GetTicks(); //Zum errechnen der Deltatime
    while (1)
    {  
        //-----------------------------------------------------------------Schauen ob das Level abgeschlossen wurde
        bool everythingDone = true;
        for (pair<bool*, int*> questScroller : levelQuests)
        {
            if (*questScroller.first == false)
                everythingDone = false;
        }
        if (everythingDone == true)
        {
            deleteTheWholeLevel();
            return 0;
        }
        if(currentSaveFile->getLives() <= 0)
        {
            gameOverScreen();
            deleteTheWholeLevel();
            alleSaveFiles = CSavefile::EinlesenDerSpeicherdaten();
            return 1;
        }
        //-----------------------------------------------------------------
        Uint32 lastTime = currentTime;
        currentTime = SDL_GetTicks();
        if (deltaTime == -1) //Wenn das Spiel pausiert ist, würde die DeltaTime in die Höhe springen. Mithilfe der Setmethode setze ich sie dann auf -1 und das wird hier abgefangen und eine neue und niedrige Deltatime wird berechnet
        {
            deltaTime = 0;    //Sonst lande ich in einem endlosen Loop
            y_axis = 0;     //Movement-Anomalien verhindern
            x_axis = 0;
            continue;
        }        
        deltaTime = (currentTime - lastTime);
        while (SDL_PollEvent(&input) > 0)
        {
            switch (input.type)
            {
            case SDL_QUIT:
                exit(0);
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

        if (deltaTime > 140) //Limit DeltaTime, damit man nicht durch wände laufen kann
        {
            deltaTime = 140;
        }

        spielerPointer->animation(y_axis, x_axis, deltaTime);   //Neuer Frame für den Player
        spielerPointer->bewegen(y_axis * deltaTime * 0.225, x_axis * deltaTime * 0.225); //Neue location für den Player
        SDL_RenderCopy(renderer, currentMap->getTexture(), NULL, currentMap->getPosition());        
        
        NPC_Pathfinding(deltaTime * 0.1);   //alle NPC's werden bewgegt
        for (auto cursor : listeVonEntitys)
        {
            if(cursor != nullptr)
                cursor->renderer(renderer);
        }

        int proCounter = 0;

        for (auto cursor : listeVonEntitys)
            if (cursor != nullptr)
            {
                cursor->update(0, 0);
                if (cursor->getTag() == "projectile")
                {
                    proCounter++;
                }
            }

        //std::cout << proCounter << std::endl;

        for (auto cursor : allProjectiles)
        {
            //cursor->update(0, 0);
            //cursor->renderer(renderer);
        }

        spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern

      
        //for (auto cursor : currentMap->getListeVonEntitys())
        //{
        //    SDL_RenderFillRect(renderer, cursor->getBounds());
        //}
        //for(auto cursor : listeVonEntitys)
        //SDL_RenderDrawRect(renderer, cursor->getBounds());
        //SDL_RenderDrawRect(renderer, spielerPointer->getBounds());
        //SDL_RenderDrawRect(renderer, spielerPointer->getFootSpace());
 

        SDL_RenderCopy(renderer, currentMap_TopLayer->getTexture(), NULL, currentMap_TopLayer->getPosition());
        renderHUD();
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

    }


}

void CGamemaster::initLevel0()
{
    SDL_RenderClear(renderer);
    TTF_Font* font;

    font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 30);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
    }

    SDL_Surface* text;
    SDL_Texture* text_texture;
    SDL_Color color = { 255, 255, 255 }; // Set color to white

    text = TTF_RenderText_Blended_Wrapped(font, "Hallo junger Held, du weisst es vielleicht noch nicht, aber du wurdest von einer höheren Macht auserkoren!", color, SCREEN_WIDTH - 25);
    if (!text) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 50,  text->w, text->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_RenderPresent(renderer);
    SDL_Event e;
    while (SDL_PollEvent(&e) >= 0)
    {
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            break;
        else if (e.type == SDL_QUIT)
            exit(0);
        SDL_Delay(100);
    }
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);
    SDL_RenderClear(renderer);


    text = TTF_RenderText_Blended_Wrapped(font, "Du kannst dich sehr glücklich schätzen, Ruhm und Ehre werden dein sein und du wirst Menschen in Not helfen.....sofern du erfolgreich bist", color, SCREEN_WIDTH - 25);
    if (!text) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 50,  text->w, text->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_RenderPresent(renderer);
    while (SDL_PollEvent(&e) >= 0)
    {
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            break;
        else if (e.type == SDL_QUIT)
            exit(0);
        SDL_Delay(100);
    }
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);

    char nameTemp[100] = "\0";
    int charCounter = 0;
    while (SDL_PollEvent(&e) >= 0)
    {
        SDL_RenderClear(renderer);
        text = TTF_RenderText_Blended_Wrapped(font, "Aber jetzt erstmal die Förmlichkeiten, wie heisst du denn eigentlich überhaupt? (ENTER um zu bestätigen)", color, SCREEN_WIDTH - 25);
        if (!text) {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 50,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);

        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text);
        if (nameTemp[0] != '\0')
        {
            text = TTF_RenderText_Blended_Wrapped(font, nameTemp, color, SCREEN_WIDTH - 25);
            text_texture = SDL_CreateTextureFromSurface(renderer, text);
            dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT - 70,  text->w, text->h };
            SDL_RenderCopy(renderer, text_texture, NULL, &dest);
            SDL_DestroyTexture(text_texture);
            SDL_FreeSurface(text);
        }
        SDL_RenderPresent(renderer);

        if (charCounter > 0 && e.key.keysym.sym == SDLK_RETURN)
            break;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_RETURN)
        {

            char temp = detectKey(e);
            if (temp == '\b')           //Wenn Backspace gedrückt wurde, wird ein \0 geschrieben und zwar an die stelle vorher
            {
                temp = '\0';
                charCounter--;
            }
            if (charCounter < 10)
            {
                if (charCounter < 0)        //Wir wollen nicht den Speicher überschreiben
                    charCounter = 0;

                nameTemp[charCounter] = temp;

                if (temp != '\0')        //Wir dürfen kein \0 mitten im String stehen haben
                    charCounter++;
            }
        }
        else if (e.type == SDL_QUIT)
            exit(0);
    }

    string nameString = nameTemp;
    strcat_s(nameTemp, "! Was ein schöner name und das erzähle ich sicherlich nicht jedem.");
    char schwierigkeitsgradTemp[] = "0";
    while (SDL_PollEvent(&e) >= 0)
    {
        SDL_RenderClear(renderer);
        text = TTF_RenderText_Blended_Wrapped(font, nameTemp, color, SCREEN_WIDTH - 10);
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 180,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text);

        text = TTF_RenderText_Blended_Wrapped(font, "Erzähle mir etwas über dich, du bist ein..?                                                                                         beinharter Typ(1)                                          Normalo(2)                                             Dreikäsehoch(3)                                                                   Das Spielerlebnis könnte abhängig von deiner Antwort variieren.", color, SCREEN_WIDTH - 25);
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 40,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text);

        if (schwierigkeitsgradTemp[0] != '0')
        {
            text = TTF_RenderText_Blended_Wrapped(font, schwierigkeitsgradTemp, color, SCREEN_WIDTH - 25);
            text_texture = SDL_CreateTextureFromSurface(renderer, text);
            dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT - 70,  text->w, text->h };
            SDL_RenderCopy(renderer, text_texture, NULL, &dest);
            SDL_DestroyTexture(text_texture);
            SDL_FreeSurface(text);
        }
        SDL_RenderPresent(renderer);

        if (e.type == SDL_KEYDOWN)
        {
            char temp = detectKey(e);
            if (temp >= '1' && temp <= '3')
            {
                schwierigkeitsgradTemp[0] = temp;
            }
            if (e.key.keysym.sym == SDLK_RETURN)
            {
                if (schwierigkeitsgradTemp[0] >= '1' && schwierigkeitsgradTemp[0] <= '3')
                    break;
            }
        }
        else if (e.type == SDL_QUIT)
            exit(0);
    }

    SDL_RenderClear(renderer);
    text = TTF_RenderText_Blended_Wrapped(font, "Deine Geschichte beginnt an dem Ort den Viele fürchten und wenige lieben:                    die Schule. ", color, SCREEN_WIDTH - 25);
    if (!text) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 50,  text->w, text->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_RenderPresent(renderer);
    while (SDL_PollEvent(&e) >= 0)
    {
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            break;
        else if (e.type == SDL_QUIT)
            exit(0);
        SDL_Delay(100);
    }
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);

    TTF_CloseFont(font);        //Speicherplatz freigeben

    this->currentSaveFile = new CSavefile(nameString, schwierigkeitsgradTemp[0] - 48);
    this->currentSaveFile->setNextFile(alleSaveFiles);
    this->alleSaveFiles = currentSaveFile;
    this->alleSaveFiles->SchreibenDerSpeicherdaten();
}

int CGamemaster::initLevel1()
{
    //-----------------------------------------------------------------------------------------------Quest wird erstellt
    bool* tempQuest = new bool(false);
    int* tempQuestNumber = new int(1);
    levelQuests.push_back(make_pair(tempQuest, tempQuestNumber));
    //-----------------------------------------------------------------------------------------------

    SDL_Surface* tempSurface = IMG_Load(RSC_MAP1_SPRITE);    
    SDL_Rect tempBounds;
    SDL_Rect tempTextureSize;
    SDL_Rect tempTextureCoords;
    tempBounds.x = -208*4; //Extreme left of the window
    tempBounds.y = -1264; //Very top of the window
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 2;
    tempBounds.h *= 2;
    currentMap = new CMap(tempTexture, tempBounds);
    SDL_FreeSurface(tempSurface);
    
    tempSurface = IMG_Load(RSC_MAP1_SPRITE_TOP_LAYER);
    tempBounds.x = -208*4; //Extreme left of the window
    tempBounds.y = -1264; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 2;
    tempBounds.h *= 2;
    currentMap_TopLayer = new CMap(tempTexture, tempBounds);//Nur die aktuelle Karte wird abgespeichert, damit nicht unötig Speicherplatz verschwendet wird
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_PLAYER_SPRITE);
    CEntity* tempEntity;
    tempTextureCoords.w = 32;
    tempTextureCoords.h = 32;
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempBounds.x = SCREEN_WIDTH / 2; //right of the window
    tempBounds.y = SCREEN_HEIGHT / 2; //bottom of the window
    tempBounds.w = 16 * 2;
    tempBounds.h = 21 * 2;
    spielerPointer = new CPlayer(this, tempTexture, "Player", tempBounds, tempTextureCoords);
    SDL_FreeSurface(tempSurface);

    CMapEntity* tempMapEntity;
    tempBounds.x = -832 + 673 * 2;
    tempBounds.y = -1264 + 256 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 5;
    tempMapEntity = new CMapEntity(tempBounds); //Abgrenzung nord-Treppe (wird später gelöscht)
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -704;
    tempBounds.y = -1040;
    tempBounds.w = 16 * 2;
    tempBounds.h = 175 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die erste Kollisionszone wird erstellt
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -704;
    tempBounds.y = -720;
    tempBounds.w = 32 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die zweite Kollisionszone wird erstellt
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -608;
    tempBounds.y = -720;
    tempBounds.w = 191;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Untere Wand von 0255
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -672;
    tempBounds.y = -1008;
    tempBounds.w = 176 * 2;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die vierte Kollisionszone wird erstellt
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -320;
    tempBounds.y = -1040;
    tempBounds.w = 16 * 2;
    tempBounds.h = 176 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //usw
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -384;
    tempBounds.y = -720;
    tempBounds.w = 32 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds);
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -354;
    tempBounds.y = -848;
    tempBounds.w = 16 * 2;
    tempBounds.h = 64 * 2;
    tempMapEntity = new CMapEntity(tempBounds); // Schrank unten Rechts und Stück der Wand
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -608;
    tempBounds.y = -912;
    tempBounds.w = 80 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Obere Tischreihe
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -576;
    tempBounds.y = -816;
    tempBounds.w = 80 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Untere Tischreihe
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -640;
    tempBounds.y = -848;
    tempBounds.w = 16 * 2;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Lehrerpult
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -543;
    tempBounds.y = -848;
    tempBounds.w = 13 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Untere Tischreihe Klassenzimmer, hervorstehender Tisch Nr.1
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -447;
    tempBounds.y = -848;
    tempBounds.w = 13 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Untere Tischreihe Klassenzimmer, hervorstehender Tisch Nr.2
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -411;
    tempBounds.y = -944;
    tempBounds.w = 16 * 6 - 5;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Fernseher in 0255
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -411;
    tempBounds.y = -944;
    tempBounds.w = 16 * 6 - 5;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Fernseher in 0255
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 272 * 2;
    tempBounds.y = -1264 + 256 * 2;
    tempBounds.w = 16 * 32;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Nordwand des Ostflügels
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 528 * 2;
    tempBounds.y = -1264 ;
    tempBounds.w = 16 * 16;
    tempBounds.h = 32 * 3 + 10;
    tempMapEntity = new CMapEntity(tempBounds); //Nordwand des Südflügels
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 512 * 2;
    tempBounds.y = -1264;
    tempBounds.w = 16 * 2;
    tempBounds.h = 32 * 18;
    tempMapEntity = new CMapEntity(tempBounds); //Obere Westwand des Südflügels
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 144 * 2;
    tempBounds.w = 16 * 18;
    tempBounds.h = 32 * 7;
    tempMapEntity = new CMapEntity(tempBounds); //S248 + Klo
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 656 * 2;
    tempBounds.y = -1264;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 9;
    tempMapEntity = new CMapEntity(tempBounds); //Rechte Wand S249
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 16*2;
    tempBounds.y = -1264 + 256 * 2;
    tempBounds.w = 32 * 3;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Nordwand links von 0255
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832;
    tempBounds.y = -1264 + 256 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 5;
    tempMapEntity = new CMapEntity(tempBounds); //Westwand links von 0255
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 32;
    tempBounds.y = -1264 + 336 * 2;
    tempBounds.w = 32 * 32;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Südwand unter 0255
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 512*2;
    tempBounds.y = -1264 + 336 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 33;
    tempMapEntity = new CMapEntity(tempBounds); //Untere Westwand
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 528 * 2;
    tempBounds.y = -1264 + 848 * 2;
    tempBounds.w = 32 * 11;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Südlichste Wand der Schule
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 608 * 2;
    tempBounds.w = 32 * 9;
    tempBounds.h = 32 * 11;
    tempMapEntity = new CMapEntity(tempBounds); //Secret Room
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 448 * 2;
    tempBounds.w = 7 * 2;
    tempBounds.h = 32 * 8;
    tempMapEntity = new CMapEntity(tempBounds); //Obere Westwand südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 592 * 2;
    tempBounds.w = 7 * 2;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //untere Westwand südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 624 * 2;
    tempBounds.y = -1264 + 576 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Schreibtisch Lehrer südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 656 * 2;
    tempBounds.y = -1264 + 336 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 17;
    tempMapEntity = new CMapEntity(tempBounds); //Östliche Wand
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 480 * 2;
    tempBounds.w = 32 * 6;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Nordwand südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 640 * 2;
    tempBounds.y = -1264 + 496 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Blume und Tisch südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 575 * 2;
    tempBounds.y = -1264 + 512 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 3;
    tempMapEntity = new CMapEntity(tempBounds); //Tische und Bücher südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 336 * 2;
    tempBounds.w = 32 * 9;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Oberer Flur zur Treppe, Südwand
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 624 * 2;
    tempBounds.y = -1264 + 544 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Tische unten rechts südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 624 * 2;
    tempBounds.y = -1264 + 512 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Tisch oben rechts südlichster Raum
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 336 * 2;
    tempBounds.w = 7 * 2;
    tempBounds.h = 32 * 6;
    tempMapEntity = new CMapEntity(tempBounds); //Obere dünne Westwand im Flur
    currentMap->addObjectToMap(tempMapEntity);


    tempBounds.x = -832 + 626 * 2;
    tempBounds.y = -1264 + 432 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Schreibtisch Lehrer zweiter Raum des Lehrers
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 521 * 2;
    tempBounds.y = -1264 + 192 * 2;
    tempBounds.w = 26 * 1;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Tür S249 Links
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 554 * 2;
    tempBounds.y = -1264 + 192 * 2;
    tempBounds.w = 26 * 1;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Tür S249 Rechts
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 595 * 2;
    tempBounds.y = -1264 + 128 * 2;
    tempBounds.w = 28 * 2;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Schreibtisch Lehrer S249
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 96 * 2;
    tempBounds.w = 32 * 5;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Untere Tischreihe S249
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 560 * 2;
    tempBounds.y = -1264 + 64 * 2;
    tempBounds.w = 32 * 5;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Obere Tischreihe S249
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 544 * 2;
    tempBounds.y = -1264 + 64 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Linke Tischreihe S249
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 640 * 2;
    tempBounds.y = -1264 + 368 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 3;
    tempMapEntity = new CMapEntity(tempBounds); //Klausurraum 2 Tische rechts
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 576 * 2;
    tempBounds.y = -1264 + 368 * 2;
    tempBounds.w = 31 * 3;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Klausurraum 2 Tische oben links
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 576 * 2;
    tempBounds.y = -1264 + 400 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Klausurraum 1 Tisch unten links neben NPC
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 608 * 2;
    tempBounds.y = -1264 + 400 * 2;
    tempBounds.w = 29 * 1;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Klausurraum 1 Tisch unten rechts neben NPC
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 592 * 2;
    tempBounds.y = -1264 + 432 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 1;
    tempMapEntity = new CMapEntity(tempBounds); //Klausurraum 1 Tisch unter NPC
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds.x = -832 + 704 * 2;
    tempBounds.y = -1264 + 784 * 2;
    tempBounds.w = 32 * 1;
    tempBounds.h = 32 * 4;
    tempMapEntity = new CMapEntity(tempBounds); //Abgrenzung süd-Treppe
    currentMap->addObjectToMap(tempMapEntity);

    //tempSurface = IMG_Load(RSC_BANDIT_SPRITE);
    //tempBounds.x = -654;  //left of the window
    //tempBounds.y = -940; //top of the window
    //tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    //tempTextureCoords.x = 0;
    //tempTextureCoords.y = 0;
    //tempTextureCoords.w = 16;
    //tempTextureCoords.h = 16;
    //SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    //tempBounds.w = 2 * tempBounds.w / 12; // Breite geteilt durch anzahl Frames
    //tempBounds.h = 2 * tempBounds.h / 4; // Hoehe geteilt durch anzahl der Zeilen von Frames
    //tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    //listeVonEnemies.push_back(tempEntity);
    //listeVonEntitys.push_back(tempEntity);

    //tempBounds.x = -424; // left of the window
    //tempBounds.y = -780; //top of the window
    //tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    //listeVonEnemies.push_back(tempEntity);
    //listeVonEntitys.push_back(tempEntity);
    //SDL_FreeSurface(tempSurface);

    //tempSurface = IMG_Load(RSC_ANGRY_SPROUT_SPRITE);
    //tempBounds.x = -404;  //left of the window
    //tempBounds.y = -670; //top of the window
    //tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    //tempTextureCoords.x = 0;
    //tempTextureCoords.y = 0;
    //tempTextureCoords.w = 16;
    //tempTextureCoords.h = 16;
    //SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    //tempBounds.w = 32; // Breite geteilt durch anzahl Frames
    //tempBounds.h = 32; // Hoehe geteilt durch anzahl der Zeilen von Frames
    //tempEntity = new CEnemy(this, tempTexture, "ANGRY_SPROUT", tempBounds, tempTextureCoords, false, 100, 1, 0, 5, 6, 2);
    //listeVonEnemies.push_back(tempEntity);
    //listeVonEntitys.push_back(tempEntity);
    //SDL_FreeSurface(tempSurface);

    //tempBounds.x = 262;  //left of the window
    //tempBounds.y = -680; //top of the window
    //tempEntity = new CEnemy(this, tempTexture, "ANGRY_SPROUT", tempBounds, tempTextureCoords, false, 100, 1, 0, 5, 6, 2);
    //listeVonEnemies.push_back(tempEntity);
    //listeVonEntitys.push_back(tempEntity);


    tempSurface = IMG_Load(RSC_NPC_AMELIA_SPRITE);
    tempBounds.x = -404; //Extreme left of the window
    tempBounds.y = -910; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 16 * 2;
    tempBounds.h = 24 * 2;
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schuelerin", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_ALEX_SPRITE);
    tempBounds.x = -832 + 528 * 2; //Extreme left of the window
    tempBounds.y = -1264 + 92 * 2; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 23;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 16 * 2;
    tempBounds.h = 24 * 2;
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schueler", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_ADAM_SPRITE);
    tempBounds.x = -832 + 592 * 2; //Extreme left of the window
    tempBounds.y = -1264 + 384 * 2; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 16 * 2;
    tempBounds.h = 24 * 2;
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schueler", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_BOB_SPRITE);
    tempBounds.x = -832 + 640 * 2; //Extreme left of the window
    tempBounds.y = -1264 + 60 * 2; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 23;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 16 * 2;
    tempBounds.h = 24 * 2;
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Herr_Schwaiger", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);

    
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_JOHN_SPRITE);
    tempBounds.x = -832 + 640 * 2; //Extreme left of the window
    tempBounds.y = -1264 + 80 * 2; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 24;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 16 * 2;
    tempBounds.h = 23 * 2;
    tempEntity = new CJohn(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Herr_John", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);    
    SDL_FreeSurface(tempSurface);

    tempBounds.x = -832 + 640 * 2;
    tempBounds.y = -1264 + 526 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);
    
    tempBounds.x = -832 + 624 * 2;
    tempBounds.y = -1264 + 352 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -832 + 464 * 2;
    tempBounds.y = -1264 + 295 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -832 + 400 * 2;
    tempBounds.y = -1264 + 295 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -832 + 336 * 2;
    tempBounds.y = -1264 + 295 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -832 + 272 * 2;
    tempBounds.y = -1264 + 295 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -832 + 216 * 2;
    tempBounds.y = -1264 + 208 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.w = 32;
    tempBounds.h = 32 * 5;
    tempBounds.x = -832 + 704 * 2;
    tempBounds.y = -1264 + 256 * 2;
    tempEntity = new CQuestTrigger(1, this, NULL, "STAIRS_DOWN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    spielerPointer->setCurrentMap(currentMap);

    return this->gameLoop();
}

int CGamemaster::initLevel2()
{

    *this->currentSaveFile->getLevel() = 2;
    this->alleSaveFiles->SchreibenDerSpeicherdaten();

    *CNPC::getNumberOfNPCS() = 6;

    //-----------------------------------------------------------------------------------------------Quest wird erstellt
    bool* tempQuest = new bool(false);
    int* tempQuestNumber = new int(1);
    levelQuests.push_back(make_pair(tempQuest, tempQuestNumber));
    //-----------------------------------------------------------------------------------------------

    SDL_Surface* tempSurface = IMG_Load(RSC_MAP2_SPRITE);
    SDL_Rect tempBounds;
    SDL_Rect tempTextureSize;
    SDL_Rect tempTextureCoords;
    tempBounds.x = -940; //Extreme left of the window
    tempBounds.y = -615; //Very top of the window
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 2;
    tempBounds.h *= 2;
    currentMap = new CMap(tempTexture, tempBounds);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_MAP2_SPRITE_TOP_LAYER);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    currentMap_TopLayer = new CMap(tempTexture, tempBounds);//Nur die aktuelle Karte wird abgespeichert, damit nicht unötig Speicherplatz verschwendet wird
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_PLAYER_SPRITE);
    CEntity* tempEntity;
    tempTextureCoords.w = 32;
    tempTextureCoords.h = 32;
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempBounds.x = SCREEN_WIDTH / 2; //right of the window
    tempBounds.y = SCREEN_HEIGHT / 2; //bottom of the window
    tempBounds.w = 16 * 2;
    tempBounds.h = 21 * 2;
    spielerPointer = new CPlayer(this, tempTexture, "Player", tempBounds, tempTextureCoords);
    SDL_FreeSurface(tempSurface);

    CMapEntity* tempMapEntity;

    tempBounds = { -940 + 421 * 2,-615 + 224 * 2, 32 * 5,32 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Abgrenzung zum nächsten Level
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 576 * 2,-615 + 528 * 2, 32 * 9,32 * 19 };
    tempMapEntity = new CMapEntity(tempBounds); //Alle südlichen Räume
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 576 * 2,-615 + 192 * 2, 32 * 9,32 * 16 };
    tempMapEntity = new CMapEntity(tempBounds); //Klo und Fahrstuhl
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 544 * 2,-615 + 816 * 2, 32 * 2, 32 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //südliche Türen
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 528 * 2,-615 + 192 * 2, 32 * 3, 32 * 13 };
    tempMapEntity = new CMapEntity(tempBounds); //Eingang S49
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 528 * 2,-615 + 192 * 2, 32 * 1, 32 * 18 };
    tempMapEntity = new CMapEntity(tempBounds); //linke Hauswand oben
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 528 * 2,-615 + 528 * 2, 32 * 1, 32 * 19 };
    tempMapEntity = new CMapEntity(tempBounds); //linke Hauswand unten
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 256 * 2,-615 + 750 * 2, 32 * 17, 32 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Zaun ganz unten
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 244 * 2,-615 + 505 * 2, 32 * 1, 32 * 16 };
    tempMapEntity = new CMapEntity(tempBounds); //Zaun unten linke Seite
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 256 * 2,-615 + 433 * 2, 32 * 11, 32 * 5 };
    tempMapEntity = new CMapEntity(tempBounds); //WC Gebäude
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 229 * 2,-615 + 160 * 2, 32 * 12, 32 * 17 };
    tempMapEntity = new CMapEntity(tempBounds); //Zaun oben
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 720 * 2,-615 + 448 * 2, 32 * 1, 32 * 5 };
    tempMapEntity = new CMapEntity(tempBounds); //Treppe Spawn
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 448 * 2,-615 + 688 * 2, 32 * 2, 32 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Baum bei den 2 NPCS
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 432 * 2,-615 + 288 * 2, 32 * 2, 32 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Baum oben auf dem Pfad
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 323 * 2,-615 + 610 * 2, 26 * 1, 26 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Westlicher Stein unten
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 434 * 2,-615 + 371 * 2, 26 * 1, 26 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Westlicher Stein oben
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 514 * 2,-615 + 339 * 2, 26 * 1, 26 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Östlicher Stein oben
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 415 * 2,-615 + 640 * 2, 26 * 1, 26 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Östlicher Stein unten
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 421 * 2,-615 + 224 * 2, 32 * 3, 32 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Stein ganz oben am Weg
    currentMap->addObjectToMap(tempMapEntity);

    tempBounds = { -940 + 496 * 2,-615 + 224 * 2, 32 * 3, 32 * 1 };
    tempMapEntity = new CMapEntity(tempBounds); //Baum ganz oben am Weg
    currentMap->addObjectToMap(tempMapEntity);

    tempSurface = IMG_Load(RSC_BANDIT_SPRITE);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 16;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -940 + 557 * 2,-615 + 414 * 2, 16 * 2,  16 * 2 };
    tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);
    ((CEnemy*)tempEntity)->setCooldown(60 * 7);

    tempSurface = IMG_Load(RSC_NPC_ANDREAS_SPRITE);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -940 + 290 * 2,-615 + 533 * 2,16 * 2,  24 * 2 };
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schueler", tempBounds, tempTextureCoords, true);
    listeVonEntitys.push_back(tempEntity);
    spielerPointer->setCurrentMap(currentMap);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_LUKE_SPRITE);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -940 + 300 * 2,-615 + 560 * 2,16 * 2,  24 * 2 };
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schueler", tempBounds, tempTextureCoords, true);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_BOOK_SPRITE);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -940 + 544 * 2,-615 + 784 * 2,16 * 2,  24 * 2 };
    tempEntity = new CBuch(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Buch", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_FRED_SPRITE);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -940 + 464 * 2,-615 + 480 * 2,16 * 2,  24 * 2 };
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schueler", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -940 + 472 * 2,-615 + 210 * 2,16 * 2,  24 * 2 };
    tempEntity = new CNPC(this, NULL, "InnerVoice", tempBounds, tempTextureCoords, false);
    listeVonEntitys.push_back(tempEntity);

    tempBounds = { -940 + 421 * 2,-615 + 208 * 2, 32 * 5,32 * 1 };
    tempEntity = new CQuestTrigger(1, this, NULL, "ExitPath", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempSurface = IMG_Load(RSC_ANGRY_SPROUT_SPRITE);
    tempBounds.x = -940 + 464 * 2;  //left of the window
    tempBounds.y = -615 + 352 * 2; //top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 16;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 32; // Breite geteilt durch anzahl Frames
    tempBounds.h = 32; // Hoehe geteilt durch anzahl der Zeilen von Frames
    tempEntity = new CEnemy(this, tempTexture, "ANGRY_SPROUT", tempBounds, tempTextureCoords, false, 100, 1, 0, 5, 6, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    ((CEnemy*)tempEntity)->setCooldown(60 * 7);

    tempBounds.x = -940 + 380 * 2;  //left of the window
    tempBounds.y = -615 + 650 * 2; //top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempEntity = new CEnemy(this, tempTexture, "ANGRY_SPROUT", tempBounds, tempTextureCoords, false, 100, 1, 0, 5, 6, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    ((CEnemy*)tempEntity)->setCooldown(60 * 7);
    SDL_FreeSurface(tempSurface);

    tempBounds.x = -940 + 528 * 2;
    tempBounds.y = -615 + 487 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -940 + 498 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -940 + 558 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -940 + 368 * 2;
    tempBounds.y = -615 + 624 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -940 + 416 * 2;
    tempBounds.y = -615 + 688 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -940 + 272 * 2;
    tempBounds.y = -615 + 656 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -940 + 496 * 2;
    tempBounds.y = -615 + 592 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -940 + 480 * 2;
    tempBounds.y = -615 + 320 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    spielerPointer->setCurrentMap(currentMap);

    return this->gameLoop();
}

int CGamemaster::initLevel3()
{

    *this->currentSaveFile->getLevel() = 3;
    this->alleSaveFiles->SchreibenDerSpeicherdaten();

    *CNPC::getNumberOfNPCS() = 11;

    //-----------------------------------------------------------------------------------------------Quest wird erstellt
    bool* tempQuest = new bool(false);
    int* tempQuestNumber = new int(1);
    levelQuests.push_back(make_pair(tempQuest, tempQuestNumber));
    //-----------------------------------------------------------------------------------------------

    SDL_Surface* tempSurface = IMG_Load(RSC_MAP3_SPRITE);
    SDL_Rect tempBounds;
    SDL_Rect tempTextureSize;
    SDL_Rect tempTextureCoords;
    tempBounds.x = -320; //Extreme left of the window
    tempBounds.y = -800; //Very top of the window
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w *= 2;
    tempBounds.h *= 2;
    currentMap = new CMap(tempTexture, tempBounds);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_MAP3_SPRITE_TOP_LAYER);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    currentMap_TopLayer = new CMap(tempTexture, tempBounds);//Nur die aktuelle Karte wird abgespeichert, damit nicht unötig Speicherplatz verschwendet wird
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_PLAYER_SPRITE);
    CEntity* tempEntity;
    tempTextureCoords.w = 32;
    tempTextureCoords.h = 32;
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempBounds.x = SCREEN_WIDTH / 2; //right of the window
    tempBounds.y = SCREEN_HEIGHT / 2; //bottom of the window
    tempBounds.w = 16 * 2;
    tempBounds.h = 21 * 2;
    spielerPointer = new CPlayer(this, tempTexture, "Player", tempBounds, tempTextureCoords);
    SDL_FreeSurface(tempSurface);


    tempBounds.x = -320;
    tempBounds.y = -800;
    tempBounds.w = 736 * 2;
    tempBounds.h = 190 * 2;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Wald Oberseite mit Schule

    tempBounds.w = 221 * 2;
    tempBounds.h = 821 * 2;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Wald links

    tempBounds.x = -320 + 522 * 2;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Wald rechts

    tempBounds.x = -320;
    tempBounds.y = -800 + 592 * 2;
    tempBounds.w = 736 * 2;
    tempBounds.h = 190 * 2;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Wald Unterseite

    tempBounds.x = -320 + 432 * 2;
    tempBounds.y = -800 + 496 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 1;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Baum unten rechts

    tempBounds.x = -320 + 288 * 2;
    tempBounds.y = -800 + 448 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 1;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Baum unten links

    tempBounds.x = -320 + 448 * 2;
    tempBounds.y = -800 + 336 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 1;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Baum oben rechts

    tempBounds.x = -320 + 272 * 2;
    tempBounds.y = -800 + 304 * 2;
    tempBounds.w = 32 * 2;
    tempBounds.h = 32 * 1;
    currentMap->addObjectToMap(new CMapEntity(tempBounds)); //Baum oben links

    tempBounds.x = -320 + 432 * 2;
    tempBounds.y = -800 + 416 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -320 + 300 * 2;
    tempBounds.y = -800 + 330 * 2;
    tempEntity = new CCoin(this, NULL, "COIN", tempBounds, tempTextureCoords, NULL);
    listeVonEntitys.push_back(tempEntity);

    tempSurface = IMG_Load(RSC_NPC_KEVIN_SPRITE);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -320 + 360 * 2,-800 + 197 * 2,16 * 2,  24 * 2 };
    tempEntity = new CKevin(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Kevin", tempBounds, tempTextureCoords, true);
    listeVonEntitys.push_back(tempEntity);
    spielerPointer->setCurrentMap(currentMap);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_BANDIT_SPRITE);
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 16;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds = { -320 + 473 * 2,-800 + 460 * 2,16 * 2,  16 * 2 };
    tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    ((CEnemy*)tempEntity)->setCooldown(60 * 5);


    tempBounds = { -320 + 316 * 2,-800 + 266 * 2,16 * 2,  16 * 2 };
    tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    ((CEnemy*)tempEntity)->setCooldown(60 * 5);

    tempBounds = { -320 + 445 * 2,-800 + 238 * 2,16 * 2,  16 * 2 };
    tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    ((CEnemy*)tempEntity)->setCooldown(60 * 5);
    SDL_FreeSurface(tempSurface);

    spielerPointer->setCurrentMap(currentMap);

    return this->gameLoop();
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

list<CEntity*>* CGamemaster::getlisteVonEntitys()
{
    return &listeVonEntitys;
}

list<CEntity*>* CGamemaster::getlisteVonEnemies()
{
    return &listeVonEnemies;
}

void CGamemaster::NPC_Pathfinding(double deltaTime)
{
    //for (CEntity* cursorEntity : listeVonEntitys)
    //std::list<CEntity*>::iterator it;
    for (auto it = listeVonEntitys.begin(); it != listeVonEntitys.end(); it++)
    {
        //cursorEntity->entityPathfinding(deltaTime);
        (*it)->entityPathfinding(deltaTime);

        if ((*it)->getTag() == "projectile")
        {
            if (((Projectile*)(*it))->getKillFlag())
            {
                std::cout << "Entity count:" << listeVonEntitys.size() << std::endl;
                std::cout << "Removing projectile" << std::endl;
                delete (*it);
                listeVonEntitys.erase(it--);
                std::cout << "Entity count:" << listeVonEntitys.size() << std::endl;
            }
        }
        //it->entityPathfinding(deltaTime);
    }
    return;
}
void CGamemaster::titlescreen()
{
    TTF_Font* font;
    SDL_Event e;
    SDL_Surface* text;    // Set color to white
    SDL_Color colorNG = { 255, 255, 255 };      //Damit können wir einen "Hover-Effekt" machen, der den jeweiligen Button highligted
    SDL_Color colorSS = { 255, 255, 255 };
    SDL_Color colorQG = { 255, 255, 255 };
    //SDL_Color colorEG = { 255, 255, 255 };
    while (SDL_PollEvent(&e) >= 0)
    { 
        SDL_RenderClear(renderer);              
        SDL_Surface* tempSurface = IMG_Load(RSC_BACKGROUND_OF_TITLESCREEN);
        SDL_Rect tempBounds;
        SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        tempBounds.x = 0;
        tempBounds.y = 0;
        tempBounds.w = SCREEN_WIDTH;
        tempBounds.h = SCREEN_HEIGHT;
        SDL_RenderCopy(renderer, tempTexture, NULL, &tempBounds);
        SDL_DestroyTexture(tempTexture);
        SDL_FreeSurface(tempSurface);

        
        font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 50);
        if (!font) 
        {
            cout << "Failed to load font: " << TTF_GetError() << endl;
        }


        text = TTF_RenderText_Blended(font, "Findet Kevin", { 255, 255, 255 });
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        SDL_Texture* text_texture;
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_Rect dest = { SCREEN_WIDTH / 2 - text->w * 0.5,  SCREEN_HEIGHT / 2 - 120,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);       //Memory management
        SDL_FreeSurface(text);              
        TTF_CloseFont(font);
        font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 24);

        /* New game Button */
        text = TTF_RenderText_Blended(font, "New Game", colorNG);
        if (!text) {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_Rect startGame = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 50,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &startGame);
        SDL_DestroyTexture(text_texture);       //Memory management
        SDL_FreeSurface(text);

        /* Select Savefile Button */
        text = TTF_RenderText_Blended(font, "Select Savefile", colorSS);
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_Rect selectSavefile = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 15,  text->w, text->h };

        SDL_RenderCopy(renderer, text_texture, NULL, &selectSavefile);
        SDL_DestroyTexture(text_texture);                   //Memory management
        SDL_FreeSurface(text);

        /* Close game Button */
        text = TTF_RenderText_Blended(font, "Close Game", colorQG);
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_Rect closeButton = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 + 20,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &closeButton);
        SDL_DestroyTexture(text_texture);                   //Memory management
        SDL_FreeSurface(text);

        /* -------------- Test Endgame --------------------- */
        /*
        text = TTF_RenderText_Blended(font, "Endgame", colorEG);
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_Rect endgameButton = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 + 55,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &endgameButton);
        */
        /* ------------------------------------------------ */

        SDL_RenderPresent(renderer);
        SDL_Rect cursor_Hitbox;
        SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);
        cursor_Hitbox.w = 8;
        cursor_Hitbox.h = 4;
        //SDL_DestroyTexture(text_texture);      //Memory management
        //SDL_FreeSurface(text);
        TTF_CloseFont(font);

        colorNG = { 255, 255, 255 };
        colorSS = { 255, 255, 255 };
        colorQG = { 255, 255, 255 };
        //colorEG = { 255, 255, 255 };

        SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (SDL_HasIntersection(&cursor_Hitbox, &closeButton))
                return;

            if (SDL_HasIntersection(&cursor_Hitbox, &startGame))
            {
                this->initLevel0();
                if (!this->initLevel1())
                    if (!this->initLevel2())
                        if (!this->initLevel3())
                        {
                            IKGameLogic* ik = new IKGameLogic(renderer, window, currentSaveFile, alleSaveFiles);
                            ik->init();
                            delete ik;
                        }
            }

            if (SDL_HasIntersection(&cursor_Hitbox, &selectSavefile))
            {
                this->selectSavefile();
            }

            /*
            if (SDL_HasIntersection(&cursor_Hitbox, &endgameButton))
            {
                cout << "Entering endgame!" << endl;
                IKGameLogic* ikgamelogic = new IKGameLogic(renderer, window, currentSaveFile);
                ikgamelogic->init();
                //return;
            }
            */
        }

        if (SDL_HasIntersection(&cursor_Hitbox, &closeButton))  //Farbe der Buttons wird geändert
           colorQG = { 100, 255, 100};
        if (SDL_HasIntersection(&cursor_Hitbox, &startGame))
            colorNG = { 100, 255, 100 };
        if (SDL_HasIntersection(&cursor_Hitbox, &selectSavefile))
            colorSS = { 100, 255, 100 };
        /*
        if (SDL_HasIntersection(&cursor_Hitbox, &endgameButton))
            colorEG = { 100, 255, 100 };

        */

        if (e.type == SDL_QUIT)
           return;
    }
    
}

void CGamemaster::selectSavefile()
{
    TTF_Font* font;
    SDL_Event e;
    SDL_Surface* text;    // Set color to white
    SDL_Rect forwardButton, backwardsButton, saveFileArea, sortButtonScore, sortButtonName;
    forwardButton.x = 263;
    forwardButton.y = 480;
    forwardButton.w = 123;
    forwardButton.h = 36;
    backwardsButton = forwardButton;
    forwardButton.x = 423;

    saveFileArea.x = 33;        //Wenn sich die Maus in diesem Bereich befindet wird ein Saveslot ausgewählt
    saveFileArea.y = 138;
    saveFileArea.w = 734;
    saveFileArea.h = 320;

    sortButtonScore.x = 84;
    sortButtonScore.y = 475;
    sortButtonScore.w = 118;
    sortButtonScore.h = 46;

    sortButtonName = sortButtonScore;
    sortButtonName.x = 598;

    int pages = 0;
    while (SDL_PollEvent(&e) >= 0)
    {
        SDL_RenderClear(renderer);
        SDL_Surface* tempSurface = IMG_Load(RSC_BACKGROUND_OF_SELECTSAVEFILE);
        SDL_Rect tempBounds;
        SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        tempBounds.x = 0;
        tempBounds.y = 0;
        tempBounds.w = SCREEN_WIDTH;
        tempBounds.h = SCREEN_HEIGHT;
        SDL_RenderCopy(renderer, tempTexture, NULL, &tempBounds);
        SDL_DestroyTexture(tempTexture);
        SDL_FreeSurface(tempSurface);


        font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 50);
        if (!font)
        {
            cout << "Failed to load font: " << TTF_GetError() << endl;
        }


        text = TTF_RenderText_Blended(font, "Savefiles", { 50, 50, 50 });
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        SDL_Texture* text_texture;
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_Rect dest = { SCREEN_WIDTH / 2 - text->w * 0.5,  0,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);       //Memory management
        SDL_FreeSurface(text);
        TTF_CloseFont(font);
        //-------------------------------------------HOVER_EFFEKT-----------------------------------------------------
        SDL_Rect cursor_Hitbox;
        SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);
        cursor_Hitbox.w = 8;
        cursor_Hitbox.h = 4;

        if (SDL_HasIntersection(&cursor_Hitbox, &saveFileArea))
        {
            SDL_SetRenderDrawColor(renderer, 90, 220, 90, 255);
            SDL_Rect hoverRect;
            hoverRect.w = saveFileArea.w;
            hoverRect.h = saveFileArea.h / 8;
            hoverRect.x = saveFileArea.x;
            hoverRect.y = cursor_Hitbox.y - ((cursor_Hitbox.y - 138) % hoverRect.h);
            SDL_RenderFillRect(renderer, &hoverRect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        else if (SDL_HasIntersection(&cursor_Hitbox, &sortButtonName))
        {
            SDL_SetRenderDrawColor(renderer, 105, 255, 105, 100);
            SDL_RenderDrawRect(renderer, &sortButtonName);
        }
        else if (SDL_HasIntersection(&cursor_Hitbox, &sortButtonScore))
        {
            SDL_SetRenderDrawColor(renderer, 105, 255, 105, 100);
            SDL_RenderDrawRect(renderer, &sortButtonScore);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        //-------------------------------------------Anzeigen_aller_Savefiles-----------------------------------------
        CSavefile* nextSlot = alleSaveFiles;
        for (int i = 0; i < 8 * pages ; i++)
        {
            if (nextSlot != nullptr)
                nextSlot = nextSlot->getNextFile();
        }


        font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 30);

        if (!font)
        {
            cout << "Failed to load font: " << TTF_GetError() << endl;
        }

        text = TTF_RenderText_Blended(font, "Name:                Date:                  Lvl:       Score:", { 255, 255, 255 });
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { 35,  90,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);       //Memory management
        SDL_FreeSurface(text);
        string giantMergeSpace = "Seite " + to_string(pages + 1);
        
        text = TTF_RenderText_Blended(font, giantMergeSpace.c_str(), { 35, 35, 35 });
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { SCREEN_WIDTH / 2 - text->w / 2,  560,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);       //Memory management
        SDL_FreeSurface(text);

        int counterofSlots = 0;        
        
        while (nextSlot != nullptr && counterofSlots < 8)
        {
            string tempDate = nextSlot->getCreationDate();

            giantMergeSpace = tempDate.substr(3, 7) + tempDate.substr(19) + " " + to_string(*nextSlot->getLevel()) + " " + nextSlot->getDifficultyString() + " " + to_string(*nextSlot->getTotalScore());
            text = TTF_RenderText_Blended(font, giantMergeSpace.c_str(), { 255, 255, 255 });
            if (!text)
            {
                cout << "Failed to render text: " << TTF_GetError() << endl;
            }
            text_texture = SDL_CreateTextureFromSurface(renderer, text);
            SDL_Rect saveFileSlot = { 235,  140 + 40 * counterofSlots,  text->w, text->h };
            SDL_RenderCopy(renderer, text_texture, NULL, &saveFileSlot);
            SDL_DestroyTexture(text_texture);       //Memory management
            SDL_FreeSurface(text);

            text = TTF_RenderText_Blended(font, nextSlot->getPlayername().c_str(), { 255, 255, 255 });  //Ausgabe des Namens
            if (!text)
            {
                cout << "Failed to render text: " << TTF_GetError() << endl;
            }
            text_texture = SDL_CreateTextureFromSurface(renderer, text);
            saveFileSlot = { 35,  140 + 40 * counterofSlots,  text->w, text->h };
            SDL_RenderCopy(renderer, text_texture, NULL, &saveFileSlot);
            SDL_DestroyTexture(text_texture);       //Memory management
            SDL_FreeSurface(text);


            counterofSlots++;
            nextSlot = nextSlot->getNextFile();

        }
        TTF_CloseFont(font);
        //---------------------------------------------------------------------------------------------------------


        SDL_RenderPresent(renderer);


        SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (SDL_HasIntersection(&cursor_Hitbox, &forwardButton))
                pages++;
            else if (SDL_HasIntersection(&cursor_Hitbox, &backwardsButton) && pages > 0)
                pages--;
            //-------------------------------------------------------------------------------------------------------Speicherstand wird ausgewählt
            else if (SDL_HasIntersection(&cursor_Hitbox, &saveFileArea))
            {
                nextSlot = alleSaveFiles;
                int numberOfChoosenFile = ((cursor_Hitbox.y - 138) / (saveFileArea.h / 8)) + 1 + 8 * pages;
                for (int slotCounter = 1; slotCounter < numberOfChoosenFile; slotCounter++)
                {
                    if (nextSlot->getNextFile() == nullptr)
                        return;
                    nextSlot = nextSlot->getNextFile();
                }
                font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 30);
                SDL_Event e;
                while (SDL_PollEvent(&e) >= 0)
                {
                    int selectedMode = 0;
                    text = TTF_RenderText_Blended(font, "Was möchten sie mit dem", { 255, 255, 255 });
                    text_texture = SDL_CreateTextureFromSurface(renderer, text);

                    dest = { SCREEN_WIDTH / 2 - 275,  SCREEN_HEIGHT / 2 - text->h - 10,  550, text->h + 100 };
                    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
                    SDL_RenderFillRect(renderer, &dest);

                    dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - text->h,  text->w, text->h };
                    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
                    SDL_DestroyTexture(text_texture);       //Memory management
                    SDL_FreeSurface(text);

                    text = TTF_RenderText_Blended(font, "Speicherstand tun?", { 255, 255, 255 });
                    text_texture = SDL_CreateTextureFromSurface(renderer, text);
                    dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2,  text->w, text->h };
                    SDL_RenderCopy(renderer, text_texture, NULL, &dest);

                    SDL_DestroyTexture(text_texture);       //Memory management
                    SDL_FreeSurface(text);

                    text = TTF_RenderText_Blended(font, "Starten", { 105, 255, 105 });
                    text_texture = SDL_CreateTextureFromSurface(renderer, text);
                    dest = { 185,  SCREEN_HEIGHT / 2 + 40,  text->w, text->h };


                    SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);
                    if (SDL_HasIntersection(&cursor_Hitbox, &dest))
                    {
                        selectedMode = 1;
                        SDL_SetRenderDrawColor(renderer, 50, 125, 50, 255);
                        SDL_RenderFillRect(renderer, &dest);
                    }

                    SDL_RenderCopy(renderer, text_texture, NULL, &dest);

                    SDL_DestroyTexture(text_texture);       //Memory management
                    SDL_FreeSurface(text);

                    text = TTF_RenderText_Blended(font, "Löschen", { 255, 105, 105 });
                    text_texture = SDL_CreateTextureFromSurface(renderer, text);
                    dest = { SCREEN_WIDTH - 185 - text->w,  SCREEN_HEIGHT / 2 + 40,  text->w, text->h };

                    if (SDL_HasIntersection(&cursor_Hitbox, &dest))
                    {
                        selectedMode = 2;
                        SDL_SetRenderDrawColor(renderer, 125, 50, 50, 255);
                        SDL_RenderFillRect(renderer, &dest);
                    }

                    SDL_RenderCopy(renderer, text_texture, NULL, &dest);

                    SDL_DestroyTexture(text_texture);       //Memory management
                    SDL_FreeSurface(text);
                    SDL_RenderPresent(renderer);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                    if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        switch (selectedMode)
                        {
                        case 1:
                            currentSaveFile = nextSlot;
                            switch (*currentSaveFile->getLevel())
                            {
                            case 1:
                                if (initLevel1())
                                    return;
                            case 2:
                                if (initLevel2())
                                    return;
                            case 3:
                                if (initLevel3())
                                    return;
                            case 4:
                                IKGameLogic * ik = new IKGameLogic(renderer, window, currentSaveFile, alleSaveFiles);
                                ik->init();
                                delete ik;
                                return;
                            }
                            return;

                        case 2:
                            deleteSavefile(nextSlot);
                            break;

                        default:
                            break;
                        }
                        break;
                    }
                    else if (e.type == SDL_QUIT)
                        exit(0);
                }

                TTF_CloseFont(font);
            }
            //-------------------------------------------------------------------------------------------------------
            else if (SDL_HasIntersection(&cursor_Hitbox, &sortButtonName))
                sortSavefilesByName();
            else if (SDL_HasIntersection(&cursor_Hitbox, &sortButtonScore))
            sortSavefilesByScore();
        }

        if (e.key.keysym.sym == SDLK_ESCAPE)
            break;
        if (e.type == SDL_QUIT)
            exit(0);
    }
}

char CGamemaster::detectKey(SDL_Event input)
{
    switch (input.key.keysym.sym)
    {
    case SDLK_a:
        return 'A';
    case SDLK_b:
        return 'B';
    case SDLK_c:
        return 'C';
    case SDLK_d:
        return 'D';
    case SDLK_e:
        return 'E';
    case SDLK_f:
        return 'F';
    case SDLK_g:
        return 'G';
    case SDLK_h:
        return 'H';
    case SDLK_i:
        return 'I';
    case SDLK_j:
        return 'J';
    case SDLK_k:
        return 'K';
    case SDLK_l:
        return 'L';
    case SDLK_m:
        return 'M';
    case SDLK_n:
        return 'N';
    case SDLK_o:
        return 'O';
    case SDLK_p:
        return 'P';
    case SDLK_q:
        return 'Q';
    case SDLK_r:
        return 'R';
    case SDLK_s:
        return 'S';
    case SDLK_t:
        return 'T';
    case SDLK_u:
        return 'U';
    case SDLK_v:
        return 'V';
    case SDLK_w:
        return 'W';
    case SDLK_x:
        return 'X';
    case SDLK_y:
        return 'Y';
    case SDLK_z:
        return 'Z';
    case SDLK_KP_ENTER:
    case SDLK_RETURN:
        return '\r';
    case SDLK_BACKSPACE:
        return '\b';
    case SDLK_SPACE:
        return ' ';
    case SDLK_0:
        return '0';
    case SDLK_1:
        return '1';
    case SDLK_2:
        return '2';
    case SDLK_3:
        return '3';
    case SDLK_4:
        return '4';
    case SDLK_5:
        return '5';
    case SDLK_6:
        return '6';
    case SDLK_7:
        return '7';
    case SDLK_8:
        return '8';
    case SDLK_9:
        return '9';
        break;
    }

    return '*';
}

void CGamemaster::deleteTheWholeLevel()
{
    delete currentMap_TopLayer;
    currentMap_TopLayer = nullptr;
    while (levelQuests.size() > 0)
    {
        delete levelQuests.front().first;
        delete levelQuests.front().second;
        levelQuests.remove(levelQuests.front());
    }

    while (listeVonEnemies.size() > 0)
        listeVonEnemies.remove(listeVonEnemies.front());
        
    delete spielerPointer;

    while (listeVonEntitys.size() > 0)
    {
        delete listeVonEntitys.front();
        listeVonEntitys.remove(listeVonEntitys.front());
    }

    for (auto cursor : currentMap->getListeVonEntitys())
    {
        delete cursor;
    }

    delete currentMap;
    currentMap = nullptr;
}

int CGamemaster::deleteSavefile(CSavefile* fileToDelete)
{
    CSavefile* scroller = alleSaveFiles;

    if (fileToDelete == nullptr)
        return 1;

    if (scroller == nullptr)
        return 1;

    if (scroller == fileToDelete)
    {
        alleSaveFiles = scroller->getNextFile();
        delete fileToDelete;
        alleSaveFiles->SchreibenDerSpeicherdaten();
        return 0;
    }


    while (scroller->getNextFile() != nullptr)
    {
        if (scroller->getNextFile() == fileToDelete)
        {
            scroller->setNextFile(scroller->getNextFile()->getNextFile());
            delete fileToDelete;
            alleSaveFiles->SchreibenDerSpeicherdaten();
            return 0;
        }
        scroller = scroller->getNextFile();
    }

    

    return 1;
}

CMap* CGamemaster::getMap()
{
    return currentMap;
}

void CGamemaster::renderStillFrameOfTheGame()
{
    SDL_RenderCopy(renderer, currentMap->getTexture(), NULL, currentMap->getPosition());
    spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern

    for (auto cursor : listeVonEntitys)
    {
        cursor->renderer(renderer);
    }

    SDL_RenderCopy(renderer, currentMap_TopLayer->getTexture(), NULL, currentMap_TopLayer->getPosition());
}

SDL_Renderer* CGamemaster::getRenderer()
{
    return renderer;
}

void CGamemaster::renderHUD()
{
    SDL_Surface* tempSurface = IMG_Load(RSC_HUD_LAYER);
    SDL_Rect tempBounds;
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempBounds.x = 0;
    tempBounds.y = 0;
    tempBounds.w = SCREEN_WIDTH;
    tempBounds.h = 62;
    SDL_RenderCopy(renderer, tempTexture, NULL, &tempBounds);
    SDL_DestroyTexture(tempTexture);
    SDL_FreeSurface(tempSurface);

    TTF_Font * font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 30);
    if (!font)
    {
        cout << "Failed to load font: " << TTF_GetError() << endl;
    }
    SDL_Surface* text;

    text = TTF_RenderText_Blended(font, to_string(currentSaveFile->getLives()).c_str(), { 179, 28, 28 });
    if (!text)
    {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    SDL_Texture* text_texture;
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect dest = {50 - text->w,  12,  text->w, text->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_DestroyTexture(text_texture);       //Memory management
    SDL_FreeSurface(text);

    text = TTF_RenderText_Blended(font, to_string(*currentSaveFile->getTotalScore()).c_str(), { 255, 255, 255 });
    if (!text)
    {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    dest = { 605,  13,  text->w, text->h };
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_DestroyTexture(text_texture);       //Memory management
    SDL_FreeSurface(text);

    TTF_CloseFont(font);
}

void CGamemaster::setDeltaTime(double deltaTime)
{
    this->deltaTime = deltaTime;
}

void CGamemaster::sortSavefilesByName()
{
    if (alleSaveFiles == nullptr || alleSaveFiles->getNextFile() == nullptr)
        return;

    list <CSavefile*> listeVonSavefiles;
    while (alleSaveFiles != nullptr)
    {
        listeVonSavefiles.push_back(alleSaveFiles);
        alleSaveFiles = alleSaveFiles->getNextFile();
    }

    
    while (listeVonSavefiles.size() > 0)
    { 
        CSavefile* temp = listeVonSavefiles.front();
        for (auto cursor : listeVonSavefiles)
        {
            if (cursor->getPlayername() > temp->getPlayername())
                temp = cursor;
        }
        temp->setNextFile(alleSaveFiles);
        alleSaveFiles = temp;
        listeVonSavefiles.remove(temp);
    }
    alleSaveFiles->SchreibenDerSpeicherdaten();

}

void CGamemaster::sortSavefilesByScore()
{
    if (alleSaveFiles == nullptr || alleSaveFiles->getNextFile() == nullptr)
        return;

    list <CSavefile*> listeVonSavefiles;
    while (alleSaveFiles != nullptr)
    {
        listeVonSavefiles.push_back(alleSaveFiles);
        alleSaveFiles = alleSaveFiles->getNextFile();
    }


    while (listeVonSavefiles.size() > 0)
    {
        CSavefile* temp = listeVonSavefiles.front();
        for (auto cursor : listeVonSavefiles)
        {
            if (*cursor->getTotalScore() < *temp->getTotalScore())
                temp = cursor;
        }
        temp->setNextFile(alleSaveFiles);
        alleSaveFiles = temp;
        listeVonSavefiles.remove(temp);
    }
    alleSaveFiles->SchreibenDerSpeicherdaten();

}

void CGamemaster::gameOverScreen()
{
    SDL_RenderClear(renderer);
    SDL_Surface* tempSurface = IMG_Load(RSC_GAME_OVER_SCREEN);
    SDL_Rect tempBounds;
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempBounds.x = 0;
    tempBounds.y = 0;
    tempBounds.w = SCREEN_WIDTH;
    tempBounds.h = SCREEN_HEIGHT;
    SDL_RenderCopy(renderer, tempTexture, NULL, &tempBounds);
    SDL_RenderPresent(renderer);
    SDL_Event e;
    while (SDL_PollEvent(&e) >= 0)
    {
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            break;
        else if (e.type == SDL_QUIT)
            exit(0);
        SDL_Delay(100);
    }

    SDL_DestroyTexture(tempTexture);
    SDL_FreeSurface(tempSurface);
}

