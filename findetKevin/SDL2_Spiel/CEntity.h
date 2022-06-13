#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include <list>
struct structForWalkingDirections
{
	int xDirection;
	int yDirection;
};
using namespace std;
class CGamemaster;
class CEntity
{
protected:
	bool moving;
	SDL_Rect bounds;		//Wiegroﬂ ist das Entity
	SDL_Rect textureCoords;	//Wo sind die Texturen auf dem Spritesheet
	SDL_Texture * texture;
	structForWalkingDirections walkingDirections;
	string tag;
	int entityID;
	static int entityCount;
	CGamemaster* game;
public:
	bool getMovingStatus();
	virtual void update(int y, int x);
	virtual int onInteract();				// 0:NPC nimmt damage, 1:NPC triggert gespr‰ch, 3:Nichts
	void renderer(SDL_Renderer* renderer);
	CEntity(CGamemaster* game, SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
	~CEntity();
	CEntity();
	structForWalkingDirections* getWalkingDirections();
	virtual int getHealth();
	virtual void setHealth(int healthpoints);
	virtual int getMaxHealth();
	void setBounds(int y, int x);//x oder y wird lediglich dazu gerechnet
	SDL_Rect* getBounds();
	int getID();
	virtual void entityPathfinding(double deltaTime);
};

