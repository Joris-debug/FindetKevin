#pragma once
#include "CEntity.h"
class CNPC :
    public CEntity
{

private:
    void  messageBox(string text);
    static int numberOfNPCs;
    int dialogueNumber;
    bool hasTalkedToThePlayer;	 // Dieses Attribut hat keine der Tochterklassen, Gegner und Projektile reden nicht
public:
    CNPC(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
    int onInteract();
    bool* getHasTalkedToThePlayer();
    void update(int y, int x);
};

