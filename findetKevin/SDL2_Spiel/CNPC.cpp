#include "CNPC.h"

CNPC::CNPC(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CEntity(textureTemp, tag, bounds, textureCoords, moving)
{
	this->hasTalkedToThePlayer = false;
}

int CNPC::onInteract()
{
	if (*getHasTalkedToThePlayer() == false)
	{
		cout << "Ich bin NPC " << getID() << endl;
		hasTalkedToThePlayer = true;
	}
	return 1;
}

bool* CNPC::getHasTalkedToThePlayer()
{
	return &hasTalkedToThePlayer;
}