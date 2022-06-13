#include "CPlayer.h"
#include "CGamemaster.h";
CPlayer::CPlayer(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords) : CEntity(game ,textureTemp, tag, bounds, textureCoords, false)
{
    this->score = 0;
	healItems = 0;
	healthPoints = 20; 
    footSpace.x = game->getWidthOfWindow() / 2; 
    footSpace.y = game->getHeigthOfWindow() / 2 + 14*2; 
    footSpace.w = 16 * 2;
    footSpace.h = 8 * 2;
}

void CPlayer::setCurrentMap(CMap *map)
{
    this->currentmap = map;
}

SDL_Rect* CPlayer::getFootSpace()
{
    return &footSpace;
}

int CPlayer::bewegen(int y, int x)
{
    bool x_collision = true, y_collision = true;
    SDL_Rect* temp;
    footSpace.x += x;   //Zuerst bewege ich den Spieler und schaue ob er gegen etwas stoesst, danach erst bewege ich wirklich alle anderen Elemente
    for (auto cursor : currentmap->getListeVonEntitys())
    {
        if (SDL_HasIntersection(&footSpace, cursor->getBounds()))
        {
            x_collision = false;
        }
    }

    //for (auto cursor : game->getlisteVonEntitys())          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
    //{
    //    if (SDL_HasIntersection(&footSpace, cursor->getBounds()))
    //    {
    //        x_collision = false;
    //    }

    //}

    footSpace.x -= x;       //Bewegung wird Rueckgaengig gemacht
    x *= -1;    //Wir wollen die Welt um den Spieler bewegen und nicht den Spieler durch die Welt, darum invertieren wir die Richtungsvektoren
    
    if (x_collision)    //Alle MapEntities werden bewegt
    {
        for (auto cursor : currentmap->getListeVonEntitys())
        {
            temp = cursor->getBounds();
            temp->x += x;
        }

    }

    footSpace.y += y;   //Zuerst bewege ich den Spieler und schaue ob er gegen etwas stoesst, danach erst bewege ich wirklich alle anderen Elemente
    for (auto cursor : currentmap->getListeVonEntitys())
    {
        if (SDL_HasIntersection(&footSpace, cursor->getBounds()))
        {
            y_collision = false;
        }
    }

    //for (auto cursor : game->getlisteVonEntitys())          //Diese Schleife schaut nach mit welchen anderen Entities ich kollidiere
    //{
    //    if (SDL_HasIntersection(&footSpace, cursor->getBounds()))
    //    {
    //        y_collision = false;
    //    }

    //}

    footSpace.y -= y;
    y *= -1;
    if (y_collision)
    {
        for (auto cursor : currentmap->getListeVonEntitys())
        {
            temp = cursor->getBounds();
            temp->y += y;
        }
    }

    game->moveMaps(x * x_collision, y * y_collision);
    game->moveEntitys(x * x_collision, y * y_collision);

    for (auto cursor : *game->getlisteVonEntitys())                  //Diese Schleife schaut nach mit welchem Gegnern ich kollidiere
    {
        if (SDL_HasIntersection(&bounds, cursor->getBounds()))
        {
            int collisionID = cursor->getID();
            for (auto cursor : *game->getlisteVonEntitys())
            {
                if (cursor->getID() == collisionID)
                {
                    if (cursor->onInteract() == 0)                 //Wenn der Gegner durch interaction schaden nimmt,....
                        if (cursor->getHealth() <= 0)
                        {
                            game->getlisteVonEntitys()->remove(cursor);             //Gegner getroffen, ZEIT IHN ZU VERNICHTEN MUHAHAHAHAHA (Capslock war an, ups)
                            if (typeid(*cursor) == typeid(CEnemy))//Ich nehme typeid um möglichst schnell zu schauen ob das objekt in der liste ist                                             
                            game->getlisteVonEnemies()->remove(cursor);
                            delete cursor;
                        }
                    return 1;
                }
            }

        }
        else
        {
            cursor->setHealth(cursor->getMaxHealth()); //Gegner die nicht berührt werden, müssen wieder geheilt werden
            if (typeid(*cursor) == typeid(CNPC))
            { 
                CNPC* temp_Pointer_NPC = dynamic_cast<CNPC*>(cursor);   //Damit kann ich einen CEntity Pointer zu einem CNPC Pointer umwandeln um so die Funktion der Tochterklasse aufzurufen
                *temp_Pointer_NPC->getHasTalkedToThePlayer() = false; // Jetzt kann das Gespräch wieder getriggert werden
            }
        }
    }
    return 0;
}

void CPlayer::animation(int y, int x, double deltaTime)
{
    int totalFrames = 8;   // Animation besteht jeweils aus 6 sprites
    int delayPerFrame = 130;
    int movingDirection = 0;

    if (x > 0 )
        movingDirection = 0; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
    else
        movingDirection = 8;// Anfangsprite ist 8 weiter Rechts auf dem Spritesheet


    int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
    textureCoords.x = frame * textureCoords.w;
    textureCoords.y = 64;
    if (y == 0 && x == 0)
    {       
        textureCoords.x = 0 + 32 *((SDL_GetTicks() / delayPerFrame) % 13);
        textureCoords.y = 0;
        textureCoords.h = 32;
        textureCoords.w = 32;        
    }
    
}