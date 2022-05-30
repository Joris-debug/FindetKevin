#pragma once
#include "CEntity.h"
class CNPC :
    public CEntity
{

private:
    bool hasTalkedToThePlayer;	 // Dieses Attribut hat keine der Tochterklassen, Gegner und Projektile reden nicht
public:
    CNPC(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
    int onInteract();
    bool* getHasTalkedToThePlayer();
};
