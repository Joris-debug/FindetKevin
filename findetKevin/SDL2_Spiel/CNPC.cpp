#include "CNPC.h"

CNPC::CNPC()
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