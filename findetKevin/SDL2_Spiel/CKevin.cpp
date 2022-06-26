#include "CKevin.h"
#include "CGamemaster.h"
#include <fstream>
#include <string>
CKevin::CKevin(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CNPC(game, textureTemp, tag, bounds, textureCoords, false)
{

}

int CKevin::onInteract()
{
	if (*getHasTalkedToThePlayer() == false)
	{

		string lineOfDialogue;

		ifstream dialogueFile;
		dialogueFile.open("NPC_Dialogue.csv");// always check whether the file is open
		if (dialogueFile.is_open())
		{
			while (getline(dialogueFile, lineOfDialogue))
			{
				if (lineOfDialogue.substr(0, lineOfDialogue.find(";")) == to_string(dialogueNumber))
					messageBox(lineOfDialogue.substr(lineOfDialogue.find(";") + 1));
			}

		}
		else cout << "Unable to open file";
		dialogueFile.close();
		hasTalkedToThePlayer = true;

		for (pair<bool*, int*> questScroller : *game->getLevelQuests())
		{
			if (*questScroller.second == 1)
				*questScroller.first = true;
		}

	}
	return 3;
}
