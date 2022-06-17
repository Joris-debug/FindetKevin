#pragma once
#include "CEntity.h"
class CQuestTrigger :    public CEntity
{
private:
	int questNumber;
public:
	CQuestTrigger(int questNumber, CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
	virtual void update(int y, int x);
	virtual int onInteract();				// 0:NPC nimmt damage, 1:NPC triggert gespräch, 3:Nichts
};

