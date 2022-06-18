#include "IKMap.h"

#include "IKPlayer.h"
#include "IKRenderLayer.h"

#include "Resources.h"

#include <iostream>

IKMap::IKMap(SDL_Renderer* renderer)
{
	m_Renderer = renderer;
	m_OffsetY = 10;
}

IKMap::~IKMap()
{
	delete m_Player;
	delete m_Simulation;
}

void IKMap::init()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 10.0f);
	m_Simulation = new b2World(gravity);
	m_Simulation->ShiftOrigin(b2Vec2(0.0f, 620.0f));

	m_Player = new IKPlayer(this);
	m_Player->init();

	m_BackgroundLayer = std::make_unique<IKRenderLayer>();

	m_BackgroundLayer->init(RSC_IK_BACKGROUND_IMAGE_OUTLINE, m_Renderer, 1, this);

	m_Gravity = 0.04f;

	Collider* floorCollider = new Collider({ 0, 370, 800, 220 }, m_Simulation);  // x, y, w, h
	m_BackgroundLayer->getColliders().push_back(floorCollider);

	//Collider* ceilingCollider = new Collider({ 0, 160, 800, 20 }, m_Simulation);  // x, y, w, h
	//m_BackgroundLayer->getColliders().push_back(ceilingCollider);

	//Collider* randomCollider = new Collider({ 0, 300, 100, 100 }, m_Simulation);  // x, y, w, h
	//m_BackgroundLayer->getColliders().push_back(randomCollider);
}

void IKMap::update(double dt)
{
	//std::cout << dt << std::endl;
	printf("%.5f \n", 1.0f / 60.0f);
	m_Simulation->Step(1.0f/60.0f, 8, 3);
	m_Player->update(dt);
	m_BackgroundLayer->update(dt);
}

void IKMap::render()
{
	m_BackgroundLayer->render(true);
	m_Player->render();
}
