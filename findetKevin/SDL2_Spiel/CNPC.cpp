#include "CNPC.h"
#include "CGamemaster.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Resources.h"
#include <SDL_ttf.h>
int CNPC::numberOfNPCs = 0;

void CNPC::messageBox(string text)
{
	SDL_RenderClear(game->getRenderer());
	game->renderStillFrameOfTheGame();
	game->renderHUD();
	SDL_Renderer* renderer = game->getRenderer();
	SDL_Surface* tempSurface = IMG_Load(RSC_BACKGROUND_OF_TEXTBOX);	
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_Rect textBoxBounds;

	SDL_QueryTexture(tempTexture, NULL, NULL, &textBoxBounds.w, &textBoxBounds.h);
	textBoxBounds.x = 0;
	textBoxBounds.y = game->getHeigthOfWindow()- textBoxBounds.h - 10;
	SDL_RenderCopy(renderer, tempTexture, NULL, &textBoxBounds);
	SDL_DestroyTexture(tempTexture);
	SDL_FreeSurface(tempSurface);
	
	//_______________________________________________________________________ BEGINN TEXT
	SDL_Surface* text_surface;
	SDL_Texture* text_texture;
	SDL_Color color = { 255, 255, 255 }; // Set color to white

	TTF_Font* font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 30);
	if (!font) {
		cout << "Failed to load font: " << TTF_GetError() << endl;
	}

	text_surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, game->getWidthOfWindow() - 40);
	if (!text_surface) {
		cout << "Failed to render text: " << TTF_GetError() << endl;
	}
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect dest = { 20,  game->getHeigthOfWindow() - 165,  text_surface->w, text_surface->h }; // 170 ist die Höhe des Textfensters
	SDL_RenderCopy(renderer, text_texture, NULL, &dest);
	SDL_RenderPresent(renderer);
	//_______________________________________________________________________ ENDE TEXT
	SDL_RenderPresent(renderer);
	SDL_Delay(750);
	SDL_Event input;
	while (SDL_PollEvent(&input) > 0);
	while (SDL_PollEvent(&input) >= 0)
	{
		if (input.type == SDL_KEYDOWN || input.type == SDL_MOUSEBUTTONDOWN)
			break;
		else if (input.type == SDL_QUIT)
			exit(0);
		SDL_Delay(100);
	}
	game->setDeltaTime(-1);
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
}

CNPC::CNPC(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CEntity(game, textureTemp, tag, bounds, textureCoords, moving)
{
	numberOfNPCs++;
	dialogueNumber = numberOfNPCs;
	hasTalkedToThePlayer = false;
}

int CNPC::onInteract()
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
	}
	return 1;
}

bool* CNPC::getHasTalkedToThePlayer()
{
	return &hasTalkedToThePlayer;
}

void CNPC::update(int y, int x)
{
	int totalFrames = 6;   // Animation besteht jeweils aus 2 sprites
	int delayPerFrame = 500;
	int movingDirection = 0;

	if (x > 0)
		movingDirection = 0; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
	if (x < 0)
		movingDirection = 12;// Anfangsprite ist drei weiter Rechts auf dem Spritesheet
	if (y < 0)
		movingDirection = 6;
	if (y > 0)
		movingDirection = 18;

	int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
	textureCoords.x = frame * textureCoords.w;
	textureCoords.y = 72;
	if (y == 0 && x == 0)
	{		
		textureCoords.x = 16 * ((SDL_GetTicks() / 2000 ^ entityID) % 4);
		textureCoords.y = 9;
		textureCoords.h = 24;
		textureCoords.w = 16;
	}
}
