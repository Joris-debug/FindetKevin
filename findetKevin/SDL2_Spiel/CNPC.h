#pragma once
#include "CEntity.h"
class CNPC :
    public CEntity
{
protected:
    static int numberOfNPCs;
    int dialogueNumber;
    bool hasTalkedToThePlayer;	 // Dieses Attribut hat keine der Tochterklassen, Gegner und Projektile reden nicht
public:
    inline int getDialogueNumber() { return dialogueNumber; }
    static inline int* getNumberOfNPCS() { return &numberOfNPCs; }
    CNPC(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
    virtual int onInteract();
    bool* getHasTalkedToThePlayer();
    void update(int y, int x);
    void  messageBox(string text);
};

