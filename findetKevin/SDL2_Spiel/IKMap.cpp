#include "IKMap.h"

#include "IKPlayer.h"

IKMap::IKMap(SDL_Renderer* renderer)
{
	m_Renderer = renderer;
}

IKMap::~IKMap()
{
	delete m_Player;
}

void IKMap::init()
{
	m_Player = new IKPlayer(this);
	m_Player->init();
}

void IKMap::render()
{
	m_Player->render();
}