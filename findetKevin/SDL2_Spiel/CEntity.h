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
class CEntity
{
protected:
	SDL_Rect bounds;
	SDL_Rect textureCoords;
	SDL_Texture * texture;
	structForWalkingDirections walkingDirections;
	string tag;
	int entityID;
	bool moving;
	static int entityCount;
public:
	bool getMovingStatus();
	virtual void update(int y, int x);
	virtual void onInteract();
	void renderer(SDL_Renderer* renderer);
	CEntity(SDL_Texture* textureTemp, string tag, SDL_Rect bounds, SDL_Rect textureCoords, bool moving);
	~CEntity();
	CEntity();
	structForWalkingDirections* getWalkingDirections();
	virtual int getHealth();
	virtual void setHealth(int healthpoints);
	virtual int getMaxHealth();
	void setBounds(int y, int x);//x oder y wird lediglich dazu gerechnet
	SDL_Rect* getBounds();
	int getID();
};

