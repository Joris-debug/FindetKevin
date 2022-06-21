#include "CBuch.h"
#include <fstream>
#include <string>
#include "CGamemaster.h"
using namespace std;
CBuch::CBuch(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CNPC(game, textureTemp, tag, bounds, textureCoords, moving)
{
}

int CBuch::onInteract()
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

		for (auto cursor : *game->getlisteVonEntitys())
		{
			if (typeid(*cursor) == typeid(CNPC))
			{
				CNPC* temp_Pointer_NPC = dynamic_cast<CNPC*>(cursor);
				if(temp_Pointer_NPC->getDialogueNumber() == 11)
				{ 
					game->getlisteVonEntitys()->remove(temp_Pointer_NPC);
					delete temp_Pointer_NPC;
					break;
				}
			}
		}
		*game->getCurrentMap()->getListeVonEntitys().front()->getBounds() = { 0,0,0,0 };
	}
	return 3;
}
