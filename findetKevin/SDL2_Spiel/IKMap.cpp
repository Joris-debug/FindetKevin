#include "IKMap.h"

#include "IKPlayer.h"
#include "IKRenderLayer.h"
#include "IKVirus.h"

//#include "Resources.h"

#include "CoordinateConversions.h"

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
	gravity.Set(0.0f, -10.0f);
	m_Simulation = new b2World(gravity);

	m_Player = new IKPlayer(this);
	m_Player->init();

	m_OrganLayer = std::make_unique<IKRenderLayer>();
	m_BorderLayer = std::make_unique<IKRenderLayer>();
	m_GeometryLayer = std::make_unique<IKRenderLayer>();
	m_Membran1Layer = std::make_unique<IKRenderLayer>();
	m_BackgroundLayer = std::make_unique<IKRenderLayer>();
	m_TopBlockerLayer = std::make_unique<IKRenderLayer>();
	m_BottomBlockerLayer = std::make_unique<IKRenderLayer>();

	//m_BackgroundLayer->init(RSC_IK_BACKGROUND_IMAGE_OUTLINE, m_Renderer, 0.8f, this);
	m_BorderLayer->init(RSC_IK_BORDER_IMAGE_OUTLINE, m_Renderer, 1.0f * 0.8f, this, 720, 0);
	m_GeometryLayer->init(RSC_IK_GEOMETRY_IMAGE, m_Renderer, 1.0f, this, 720, 0);
	m_Membran1Layer->init(RSC_IK_MEMBRAN_1_IMAGE, m_Renderer, 1.0f * 0.8f, this, 720, 0);
	m_OrganLayer->init(RSC_IK_ORGANS_IMAGE, m_Renderer, 0.768f, this, 553, 0);
	m_BackgroundLayer->init(RSC_IK_BACKGROUND_IMAGE, m_Renderer, 398.0f/720.0f * 0.78f, this, 398, 0);
	m_TopBlockerLayer->init(RSC_IK_BLOCKER_IMAGE, m_Renderer, 0.8f, this, 400, -397 * 4);
	m_BottomBlockerLayer->init(RSC_IK_BLOCKER_IMAGE, m_Renderer, 1.0f, this, 400, 720 * 4);

	m_Gravity = 0.04f;
	m_OffsetY = (D_SCREEN_HEIGHT / 2) * p2m - 182.0f;
	m_StartingOffsetY = 0.0f;

	IKVirus* virus1 = new IKVirus(m_GeometryLayer.get(), { 19 * 4, 510 * 4, 32, 32 }, m_Renderer);
	m_GeometryLayer->getViruses().push_back(virus1);

	IKVirus* virus2 = new IKVirus(m_GeometryLayer.get(), { 110 * 4, 233 * 4, 32, 32 }, m_Renderer);
	m_GeometryLayer->getViruses().push_back(virus2);

	IKVirus* virus3 = new IKVirus(m_GeometryLayer.get(), { 87 * 4, 37 * 4, 32, 32 }, m_Renderer);
	m_GeometryLayer->getViruses().push_back(virus3);

	Collider* floorCollider = new Collider({ 9 * 4 + 65 * 2, 644 * 4 + 27 * 2, 65 * 4, 27 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(floorCollider);

	Collider* ceilingCollider = new Collider({ 5 * 4 + 183 * 2, 717 * 4 + 3 * 2, 190 * 4, 3 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(ceilingCollider);

	Collider* randomCollider = new Collider({ 72 * 4 + 72 * 2, 571 * 4 + 34 * 2, 72 * 4, 34 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(randomCollider);

	Collider* collider3 = new Collider({4 * 4 + 56 * 2, 525 * 4 + 26 * 2, 56 * 4, 26 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider3);

	Collider* collider4 = new Collider({ 137 * 4 + 55 * 2, 510 * 4 + 24 * 2, 55 * 4, 24 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider4);

	Collider* collider5 = new Collider({ 47 * 4 + 62 * 2, 427 * 4 + 28 * 2, 62 * 4, 28 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider5);

	Collider* collider6 = new Collider({ 7 * 4 + 93 * 2, 340 * 4 + 27 * 2, 93 * 4, 27 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider6);

	Collider* collider7 = new Collider({ 147 * 4 + 50 * 2, 343 * 4 + 25 * 2, 50 * 4, 25 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider7);

	Collider* collider8 = new Collider({ 2 * 4 + 40 * 2, 258 * 4 + 24 * 2, 40 * 4, 24 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider8);

	Collider* collider9 = new Collider({ 97 * 4 + 36 * 2, 247 * 4 + 28 * 2, 38 * 4, 28 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider9);

	Collider* collider10 = new Collider({ 65 * 4 + 46 * 2, 185 * 4 + 24 * 2, 46 * 4, 24 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider10);

	Collider* collider11 = new Collider({ 165 * 4 + 31 * 2, 165 * 4 + 25 * 2, 31 * 4, 25 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider11);

	Collider* collider12 = new Collider({ 72 * 4 + 49 * 2, 89 * 4 + 37 * 2, 49 * 4, 37 * 4 }, m_Simulation);  // x, y, w, h
	m_GeometryLayer->getColliders().push_back(collider12);

	Collider* colliderLeft = new Collider({ 2 * 2, 37 * 4, 2 * 4, 720 * 8 }, m_Simulation);  // x, y, w, h
	m_BorderLayer->getColliders().push_back(colliderLeft);

	Collider* colliderRight = new Collider({ 198 * 4 + 2 * 2, 0 * 720 + 37 * 4, 2 * 4, 720 * 8 }, m_Simulation);  // x, y, w, h
	m_BorderLayer->getColliders().push_back(colliderRight);
}

void IKMap::update(double dt)
{
	m_Player->setLastY(m_Player->getPosB2().y);

	m_Simulation->Step(1.0f/60.0f, 8, 3);

	m_OffsetY += m_Player->getPosB2().y - m_Player->getLastY();

	m_Player->update(dt);
	m_BorderLayer->update(dt);
	m_GeometryLayer->update(dt);
	m_Membran1Layer->update(dt);
	m_OrganLayer->update(dt);
	m_BackgroundLayer->update(dt);
	m_TopBlockerLayer->update(dt);
	m_BottomBlockerLayer->update(dt);
}

void IKMap::render()
{
	m_BackgroundLayer->render(false);
	m_OrganLayer->render(false);
	m_Membran1Layer->render(false);
	m_Player->render();
	m_GeometryLayer->render(false);
	m_BorderLayer->render(false);
	m_TopBlockerLayer->render(false);
	m_BottomBlockerLayer->render(false);
}
