#pragma once
#include "CEntity.h"
class CNPC :
    public CEntity
{

private:
    bool hasTalkedToThePlayer;	 // Dieses Attribut hat keine der Tochterklassen, Gegner und Projektile reden nicht
public:
    CNPC();
    int onInteract();
    bool* getHasTalkedToThePlayer();
};

