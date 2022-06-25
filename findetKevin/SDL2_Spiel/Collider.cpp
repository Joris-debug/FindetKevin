#include "Collider.h"

#include <SDL.h>

#include "CoordinateConversions.h"

Collider::Collider(SDL_Rect other, b2World* simulation)
{
	this->m_OffsetY = 0;

	m_Simulation = simulation;

	m_Width = other.w;
	m_Height = other.h;

	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position = b2Vec2(other.x * p2m, u_sdlToB2(other.y) * p2m);

	m_Body = simulation->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(other.w / 2 * p2m, other.h / 2 * p2m);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.6f;

	m_Body->CreateFixture(&shape, 0.0f);
}

void Collider::update(float dt)
{
	//b2Vec2 pos = m_Body->GetPosition();
	//m_Box.x = pos.x;
	//m_Box.y = pos.y;
}

void Collider::render(SDL_Renderer* renderer, float yOffset, float sOffsetY)
{
	Uint8 r, g, b, a;

	SDL_Rect dstRect;

	//std::cout << renderBox.y << std::endl;

	b2Vec2 center = m_Body->GetWorldCenter();

	center.x = center.x * m2p;
	center.y = (center.y - yOffset) * m2p + sOffsetY * m2p;

	dstRect.x = center.x - (m_Width / 2);
	dstRect.y = u_b2ToSdl(center.y + (m_Height / 2));
	dstRect.w = m_Width;
	dstRect.h = m_Height;

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 100, 0, 100, 0);
	SDL_RenderFillRect(renderer, &dstRect);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

// lol

void Collider::print()
{
	//std::cout << m_Box.x << " " << m_Box.y << " " << m_Box.w << " " << m_Box.h << std::endl;
}