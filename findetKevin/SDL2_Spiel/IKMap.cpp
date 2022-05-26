#include "IKMap.h"

#include "IKPlayer.h"
#include "IKRenderLayer.h"

#include "Resources.h"

#include <iostream>

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

	m_BackgroundLayer = std::make_unique<IKRenderLayer>();

	m_BackgroundLayer->init(RSC_IK_BACKGROUND_IMAGE_OUTLINE, m_Renderer, 1);

	m_Gravity = 0.04f;

	Collider* floorCollider = new Collider({ 0, 400, 800, 220 });  // x, y, w, h
	m_BackgroundLayer->getColliders().push_back(floorCollider);

	Collider* ceilingCollider = new Collider({ 0, 200, 800, 20 });  // x, y, w, h
	m_BackgroundLayer->getColliders().push_back(ceilingCollider);
}

void IKMap::update(double dt)
{
	m_BackgroundLayer->update(dt);
	m_Player->update(dt);
}

void IKMap::render()
{
	m_Player->render();
	m_BackgroundLayer->render(false);
}
