#include "CJohn.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Resources.h"
#include "CGamemaster.h"
#include "CEnemy.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "CMap.h"
using namespace std;
CJohn::CJohn(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving) : CNPC(game, textureTemp, tag, bounds, textureCoords, moving)
{
	
}

int CJohn::onInteract()
{
	if (*getHasTalkedToThePlayer() == false)
	{

		hasTalkedToThePlayer = true;
		string tempString;
		tempString = "Moin " + game->getCurrentSaveFile()->getPlayername() + ", ich habe schon auf dich gewartet.";
		messageBox(tempString);
		messageBox("Dein Mitschüler Kevin kommt schon seit Tagen nicht mehr in die Schule.");
		messageBox("Ich habe noch eine Informatik Klausur die er Nachschreiben muss, da kommst du ins Spiel!");
		messageBox("Geh los und finde ihn, wenn du erfolgreich zurückkommst, bekommst du einen Bonus-Punkt...");

		//_______________________________________________________________________ BEGINN SCREAM
		SDL_RenderClear(game->getRenderer());
		game->renderStillFrameOfTheGame();
		SDL_Renderer* renderer = game->getRenderer();
		SDL_Surface* tempSurface = IMG_Load(RSC_BACKGROUND_OF_TEXTBOX);
		SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_Rect textBoxBounds;

		SDL_QueryTexture(tempTexture, NULL, NULL, &textBoxBounds.w, &textBoxBounds.h);
		textBoxBounds.x = 0;
		textBoxBounds.y = game->getHeigthOfWindow() - textBoxBounds.h - 10;
		SDL_RenderCopy(renderer, tempTexture, NULL, &textBoxBounds);
		SDL_DestroyTexture(tempTexture);
		SDL_FreeSurface(tempSurface);

		
		SDL_Surface* text_surface;
		SDL_Texture* text_texture;
		SDL_Color color = { 255, 255, 255 }; // Set color to white

		TTF_Font* font = TTF_OpenFont(RSC_FONT_PIXELSPLITTER, 85);
		if (!font) {
			cout << "Failed to load font: " << TTF_GetError() << endl;
		}

		text_surface = TTF_RenderText_Blended_Wrapped(font, "AAAAAAAHHHHH!", color, game->getWidthOfWindow() - 40);
		if (!text_surface) {
			cout << "Failed to render text: " << TTF_GetError() << endl;
		}
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect dest = { 20,  game->getHeigthOfWindow() - 165,  text_surface->w, text_surface->h }; // 170 ist die Höhe des Textfensters
		SDL_RenderCopy(renderer, text_texture, NULL, &dest);
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
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text_surface);
		//_______________________________________________________________________ ENDE SCREAM

		messageBox("Was war das? Also ich war es auf jeden fall nicht!");
		messageBox("Kannst du dich bitte Mal los machen und nachsehen? Ich muss noch etwas mit Herr Schwaiger bereden.");

		for (auto cursor : *game->getlisteVonEntitys())
		{
			if (cursor->getID() == 1)
			{
				game->getlisteVonEnemies()->remove(cursor);
				game->getlisteVonEntitys()->remove(cursor);
				delete cursor;
				break;
			}
		}

		SDL_Rect tempBounds = this->bounds, tempTextureCoords;
		CEntity* tempEntity;
		tempBounds.x += -96;  //left of the window
		tempBounds.y += +416; //top of the window
		tempSurface = IMG_Load(RSC_ANGRY_SPROUT_SPRITE);
		tempTexture = SDL_CreateTextureFromSurface(game->getRenderer(), tempSurface);
		tempTextureCoords.x = 0;
		tempTextureCoords.y = 0;
		tempTextureCoords.w = 16;
		tempTextureCoords.h = 16;
		tempBounds.w = 32;
		tempBounds.h = 32;
		tempEntity = new CEnemy(game, tempTexture, "ANGRY_SPROUT", tempBounds, tempTextureCoords, false, 100, 1, 0, 5, 6, 2);
		game->getlisteVonEnemies()->push_back(tempEntity);
		game->getlisteVonEntitys()->push_back(tempEntity);
		SDL_FreeSurface(tempSurface);

		tempBounds = this->bounds;
		tempSurface = IMG_Load(RSC_NPC_AMELIA_SPRITE);
		tempBounds.x += -104*2; //Extreme left of the window
		tempBounds.y += +176*2; //Very top of the window
		tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		tempTextureCoords.x = 0;
		tempTextureCoords.y = 0;
		tempTextureCoords.w = 16;
		tempTextureCoords.h = 32;
		SDL_QueryTexture(tempTexture, NULL, NULL, &tempBounds.w, &tempBounds.h); //Größe wird automatisch erkannt
		tempBounds.w = 16 * 2;
		tempBounds.h = 23 * 2;
		tempEntity = new CNPC(game, SDL_CreateTextureFromSurface(renderer, tempSurface), "SchuelerinStoryTelling", tempBounds, tempTextureCoords, false);
		game->getlisteVonEntitys()->push_back(tempEntity);
		SDL_FreeSurface(tempSurface);

		delete *game->getCurrentMap()->getListeVonEntitys().begin();	//Erstes Element wird gelöscht und dies ist die Wand zur Treppe
		
	}
	return 3;
}
