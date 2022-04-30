#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
#include <list>
#include "CPlayer.h"
#include "CMap.h"
#include "CEnemy.h"
#include "CSavefile.h"
#include <typeinfo>
class CGamemaster
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event input;
	list <CSavefile*> listeVonSavefiles;
	list <CEntity*> listeVonEntitys;
	list <CEntity*> listeVonEnemies;
	CMap* currentMap;
	CMap* currentMap_TopLayer;
	CPlayer * spielerPointer;
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 620;
	void gameLoop();
public:
	CGamemaster();
	~CGamemaster();
	void init();
	int getWidthOfWindow();
	int getHeigthOfWindow();
	void moveMaps(int x, int y);
	void moveEntitys(int x, int y);
	list <CEntity*> getlisteVonEntitys();
	int collisionDetection(int collisionID);
	void NPC_Pathfinding(double deltaTime);
	void titlescreen();
	char detectKey(SDL_Event input);
	void characterSpeech(int characterID); //Der Text der Person wird in einer externen Datei abgespeichert, daraus lesen wir sie aus
};

