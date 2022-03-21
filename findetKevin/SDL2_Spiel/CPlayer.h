#pragma once
#include "CEntity.h"
class CPlayer :
    public CEntity
{
private:
    int healthPoints;
    int healItems;
    SDL_Rect footSpace;
public:
    void onDeath();
    int bewegen(int y, int x);
    int interagieren();
    CPlayer(SDL_Texture* textureTemp, string tag, SDL_Rect bounds);
};

