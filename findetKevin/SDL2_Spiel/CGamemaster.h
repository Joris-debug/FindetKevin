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
#include "CNPC.h"
class CGamemaster
{
private:
	CSavefile* alleSaveFiles;
	CSavefile* currentSaveFile;
	double deltaTime;
	SDL_Window* window;
	SDL_Renderer* renderer;
	list <CEntity*> listeVonEntitys;
	list <CEntity*> listeVonEnemies;
	CMap* currentMap;
	CMap* currentMap_TopLayer;
	CPlayer * spielerPointer;
	list <pair<bool*, int*>> levelQuests;		//Jedes Level könnte mehrere Quests haben und braucht mindestens eine um das nächste Level zu starten
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 620;
	void gameLoop();
public:
	CGamemaster();
	~CGamemaster();
	void initLevel1();
	void initLevel2();
	int getWidthOfWindow();
	int getHeigthOfWindow();
	void moveMaps(int x, int y);
	void moveEntitys(int x, int y);
	list <CEntity*>* getlisteVonEntitys();
	list <CEntity*>* getlisteVonEnemies();
	void NPC_Pathfinding(double deltaTime);
	void titlescreen();
	void selectSavefile();
	char detectKey(SDL_Event input);
	void deleteTheWholeLevel();
	CMap* getMap();
	inline CPlayer* getPlayer() { return spielerPointer; }
	void renderStillFrameOfTheGame();
	SDL_Renderer* getRenderer();
	inline CMap* getCurrentMap() { return currentMap; }
	inline CSavefile* getCurrentSaveFile() { return currentSaveFile; }
	inline list <pair<bool*, int*>>* getLevelQuests() { return &levelQuests; }
	void setDeltaTime(double deltaTime);
};

