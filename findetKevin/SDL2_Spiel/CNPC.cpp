#include "CNPC.h"
#include "CGamemaster.h"
#include <fstream>
#include <iostream>
#include <string>
CNPC::CNPC(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CEntity(game, textureTemp, tag, bounds, textureCoords, moving)
{
	this->hasTalkedToThePlayer = false;
}

int CNPC::onInteract()
{
	if (*getHasTalkedToThePlayer() == false)
	{
		string line;
		ifstream dialogueFile;
		dialogueFile.open("NPC_Dialogue.txt");// always check whether the file is open
		if (dialogueFile.is_open())
		{		 
			while (getline(dialogueFile, line))
			{
				cout << line << endl;
			}
		}
		else cout << "Unable to open file";
		dialogueFile.close();
		hasTalkedToThePlayer = true;
	}
	return 1;
}

bool* CNPC::getHasTalkedToThePlayer()
{
	return &hasTalkedToThePlayer;
}