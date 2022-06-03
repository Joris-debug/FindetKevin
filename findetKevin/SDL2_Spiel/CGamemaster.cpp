#include "CGamemaster.h"
#include "Resources.h"
#include "CEnemy.h"
#include <SDL_ttf.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "IKGameLogic.h"

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
    spielerPointer = NULL;
}

CGamemaster::~CGamemaster()
{
    cout << endl << "GameMasterclass wurde terminiert" << endl;
    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
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
        if (deltaTime == -1) //Wenn das Spiel pausiert ist, würde die DeltaTime in die Höhe springen. Mithilfe der Setmethode setze ich sie dann auf -1 und das wird hier abgefangen und eine neue und niedrige Deltatime wird berechnet
        {
            deltaTime++;    //Sonst lande ich in einem endlosen Loop
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

        spielerPointer->animation(y_axis, x_axis, deltaTime);   //Neuer Frame für den Player
        spielerPointer->bewegen(y_axis * deltaTime * 0.225, x_axis * deltaTime * 0.225); //Neue location für den Player
        SDL_RenderCopy(renderer, currentMap->getTexture(), NULL, currentMap->getPosition());
        spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern
        
        NPC_Pathfinding(deltaTime * 0.1);   //alle NPC's werden bewgegt
        for (auto cursor : listeVonEntitys)
        {
            cursor->renderer(renderer);
        }
        //for (auto cursor : currentMap->getListeVonEntitys())
        //{
        //    SDL_RenderDrawRect(renderer, cursor->getBounds());
        //}

        SDL_RenderCopy(renderer, currentMap_TopLayer->getTexture(), NULL, currentMap_TopLayer->getPosition());
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

    }


}

void CGamemaster::init()
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

    text = TTF_RenderText_Blended_Wrapped(font, "Hallo junger Held, du weisst es vielleicht noch nicht, aber du wurdest von einer höhereren Macht auserkoren!", color, SCREEN_WIDTH-25);
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
            return;
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
            return;
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
        if(nameTemp[0]!='\0')
        { 
        text = TTF_RenderText_Blended_Wrapped(font, nameTemp, color, SCREEN_WIDTH - 25);
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT - 70,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text);
    }
    SDL_RenderPresent(renderer);

    if (  charCounter > 0 && e.key.keysym.sym == SDLK_RETURN ||charCounter == 20)
        break;
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_RETURN)
    {
        
        char temp = detectKey(e);
        if (temp == '\b')           //Wenn Backspace gedrückt wurde, wird ein \0 geschrieben und zwar an die stelle vorher
        {
            temp = '\0';            
            charCounter--;
        }
        if (charCounter < 0)        //Wir wollen nicht den Speicher überschreiben
            charCounter = 0;

        nameTemp[charCounter] = temp;

        if(temp != '\0')        //Wir dürfen kein \0 mitten im String stehen haben
            charCounter++;
        
    }
    else if (e.type == SDL_QUIT)
         return;
    }

    string nameString = nameTemp;
    strcat_s(nameTemp, "! Was ein schöner name und das erzähle ich sicherlich nicht jedem.");
    char schwierigkeitsgradTemp[] = "0";
    while (SDL_PollEvent(&e) >= 0)
    {
        SDL_RenderClear(renderer);
        text = TTF_RenderText_Blended_Wrapped(font, nameTemp, color, SCREEN_WIDTH - 25);
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 - 90,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text);

        text = TTF_RenderText_Blended_Wrapped(font,"Erzähle mir etwas über dich, bist du ein beinharter Typ(1), ein Normalo(2) oder ein Dreikäsehoch(3)? Das Spielerlebnis könnte abhängig von deiner Antwort variieren.", color, SCREEN_WIDTH - 25);
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        dest = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2,  text->w, text->h };
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
            return;
    }
    TTF_CloseFont(font);        //Speicherplatz freigeben

    CSavefile* newSavefile = new CSavefile(nameString, schwierigkeitsgradTemp[0] - 48);
    listeVonSavefiles.push_back(newSavefile);

    SDL_Surface* tempSurface = IMG_Load(RSC_MAP1_SPRITE);    
    SDL_Rect tempBounds;
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

    tempSurface = IMG_Load(RSC_CHARAKTER_SPRITE);
    CEntity* tempEntity;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempBounds.x = SCREEN_WIDTH / 2 - 8; //right of the window
    tempBounds.y = SCREEN_HEIGHT / 2; //bottom of the window
    tempBounds.w = 16 * 2;
    tempBounds.h = 24 * 2;
    spielerPointer = new CPlayer(this, tempTexture, "Player", tempBounds, tempTextureCoords);
    SDL_FreeSurface(tempSurface);

    CMapEntity* tempMapEntity;
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
    tempBounds.w = 96 * 2;
    tempBounds.h = 16 * 2;
    tempMapEntity = new CMapEntity(tempBounds); //Die dritte Kollisionszone wird erstellt
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



    tempSurface = IMG_Load(RSC_BANDIT_SPRITE);
    tempBounds.x = -654;  //left of the window
    tempBounds.y = -940; //top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 16;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 2 * tempBounds.w / 12; // Breite geteilt durch anzahl Frames
    tempBounds.h = 2 * tempBounds.h / 4; // Hoehe geteilt durch anzahl der Zeilen von Frames
    tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);

    tempBounds.x = -424; // left of the window
    tempBounds.y = -780; //top of the window
    tempEntity = new CEnemy(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Masked_Bandit", tempBounds, tempTextureCoords, true, 100, 1, 6, 4, 4, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_ANGRY_SPROUT_SPRITE);
    tempBounds.x = -404;  //left of the window
    tempBounds.y = -670; //top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 16;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 2 * tempBounds.w / 12; // Breite geteilt durch anzahl Frames
    tempBounds.h = 2 * tempBounds.h / 4; // Hoehe geteilt durch anzahl der Zeilen von Frames
    tempEntity = new CEnemy(this, tempTexture, "ANGRY_SPROUT", tempBounds, tempTextureCoords, false, 100, 1, 0, 5, 6, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);
    SDL_FreeSurface(tempSurface);

    tempBounds.x = 262;  //left of the window
    tempBounds.y = -680; //top of the window
    tempEntity = new CEnemy(this, tempTexture, "ANGRY_SPROUT", tempBounds, tempTextureCoords, false, 100, 1, 0, 5, 6, 2);
    listeVonEnemies.push_back(tempEntity);
    listeVonEntitys.push_back(tempEntity);


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
    tempBounds.h = 32 * 2;
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schuelerin", tempBounds, tempTextureCoords, true);
    listeVonEntitys.push_back(tempEntity);
    spielerPointer->setCurrentMap(currentMap);
    SDL_FreeSurface(tempSurface);

    tempSurface = IMG_Load(RSC_NPC_ALEX_SPRITE);
    tempBounds.x = -832 + 528 * 2; //Extreme left of the window
    tempBounds.y = -1264 + 128 * 2; //Very top of the window
    tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempTextureCoords.x = 0;
    tempTextureCoords.y = 0;
    tempTextureCoords.w = 16;
    tempTextureCoords.h = 32;
    SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
    tempBounds.w = 16 * 2;
    tempBounds.h = 32 * 2;
    tempEntity = new CNPC(this, SDL_CreateTextureFromSurface(renderer, tempSurface), "Schueler", tempBounds, tempTextureCoords, true);
    listeVonEntitys.push_back(tempEntity);
    spielerPointer->setCurrentMap(currentMap);
    SDL_FreeSurface(tempSurface);

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
    for (CEntity* cursorEntity : listeVonEntitys)
    {
        cursorEntity->entityPathfinding(deltaTime);
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
    SDL_Color colorEG = { 255, 255, 255 };
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
        text = TTF_RenderText_Blended(font, "Endgame", colorEG);
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_Rect endgameButton = { SCREEN_WIDTH / 2 - text->w / 2,  SCREEN_HEIGHT / 2 + 55,  text->w, text->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &endgameButton);
        /* ------------------------------------------------ */

        SDL_RenderPresent(renderer);
        SDL_Rect cursor_Hitbox;
        SDL_GetMouseState(&cursor_Hitbox.x, &cursor_Hitbox.y);
        cursor_Hitbox.w = 8;
        cursor_Hitbox.h = 4;
        SDL_DestroyTexture(text_texture);      //Memory management
        SDL_FreeSurface(text);
        TTF_CloseFont(font);

        colorNG = { 255, 255, 255 };
        colorSS = { 255, 255, 255 };
        colorQG = { 255, 255, 255 };
        colorEG = { 255, 255, 255 };

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
            if (SDL_HasIntersection(&cursor_Hitbox, &endgameButton))
            {
                cout << "Entering endgame!" << endl;
                IKGameLogic* ikgamelogic = new IKGameLogic(renderer, window);
                ikgamelogic->init();
                return;
            }
        }

        if (SDL_HasIntersection(&cursor_Hitbox, &closeButton))  //Farbe der Buttons wird geändert
           colorQG = { 100, 255, 100};
        if (SDL_HasIntersection(&cursor_Hitbox, &startGame))
            colorNG = { 100, 255, 100 };
        if (SDL_HasIntersection(&cursor_Hitbox, &selectSavefile))
            colorSS = { 100, 255, 100 };
        if (SDL_HasIntersection(&cursor_Hitbox, &endgameButton))
            colorEG = { 100, 255, 100 };

        if (e.type == SDL_QUIT)
           return;
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
        return '8';
    case SDLK_9:
        return '9';
        break;
    }

    return '*';
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

void CGamemaster::setDeltaTime(double deltaTime)
{
    this->deltaTime = deltaTime;
}

