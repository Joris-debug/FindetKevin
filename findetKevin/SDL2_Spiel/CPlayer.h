#pragma once
#include "CEntity.h"
#include "CMap.h"
class CGamemaster;
class CPlayer : public CEntity
{
private:    
    int healthPoints;
    int healItems;
    SDL_Rect footSpace;
    CMap* currentmap;
public:
    void onDeath();
    int bewegen(int y, int x);
    int interagieren();
    void animation(int y, int x, double deltaTime);
    CPlayer(CGamemaster * game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords);
    void setCurrentMap(CMap *map);
    SDL_Rect* getFootSpace();
    void addPoints(int points);
};

