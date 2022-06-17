#include "CQuestTrigger.h"
#include "CGamemaster.h"
CQuestTrigger::CQuestTrigger(int questNumber, CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CEntity(game, textureTemp, tag, bounds, textureCoords, false)
{
	this->texture = NULL;
	this->questNumber = questNumber;
}

void CQuestTrigger::update(int y, int x)
{
	return;
}

int CQuestTrigger::onInteract()
{
	for (pair<bool*, int*> questScroller : *game->getLevelQuests())
	{
		if (*questScroller.second == questNumber)
			*questScroller.first = true;
	}
	return 3;
}
