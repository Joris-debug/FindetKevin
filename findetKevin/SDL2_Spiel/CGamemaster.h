#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include "CGamemaster.h"
#include <list>
#include <set>
#include "CPlayer.h"
#include "CMap.h"
#include "CEnemy.h"
#include "CSavefile.h"
#include <typeinfo>
#include "CNPC.h"
//#include "Projectile.h"

class Projectile;

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
	std::set<Projectile*> allProjectiles;
	CMap* currentMap;
	CMap* currentMap_TopLayer;
	CPlayer * spielerPointer;
	list <pair<bool*, int*>> levelQuests;		//Jedes Level könnte mehrere Quests haben und braucht mindestens eine um das nächste Level zu starten
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 620;
	int gameLoop();
public:
	CGamemaster();
	~CGamemaster();
	void initLevel0();
	int initLevel1();
	int initLevel2();
	int initLevel3();
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
	int deleteSavefile(CSavefile* fileToDelete);
	CMap* getMap();
	inline CPlayer* getPlayer() { return spielerPointer; }
	void renderStillFrameOfTheGame();
	SDL_Renderer* getRenderer();
	void renderHUD();
	inline CMap* getCurrentMap() { return currentMap; }
	inline CSavefile* getCurrentSaveFile() { return currentSaveFile; }
	inline list <pair<bool*, int*>>* getLevelQuests() { return &levelQuests; }
	void setDeltaTime(double deltaTime);
	void sortSavefilesByName();
	void sortSavefilesByScore();
	void gameOverScreen();
	inline std::set<Projectile*>* getAllProjectiles() { return &allProjectiles; };
};

