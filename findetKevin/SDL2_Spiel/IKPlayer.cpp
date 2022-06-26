#include "IKPlayer.h"

#include "SDL_image.h"

#include "Collider.h"
#include "IKRenderLayer.h"

#include <iostream>
#include <list>

#include "Resources.h"
#include "CoordinateConversions.h"

IKPlayer::IKPlayer(IKMap* map)
{
    m_Width = 16 * 2;
    m_Height = 21 * 2;
    //m_StartingX = 800 / 2 - (m_Width / 2)+10;
    //m_StartingY = 620 / 2 - (m_Height / 2);

    m_StartingX = 100;
    m_StartingY = u_sdlToB2(100);

    m_Map = map;

    std::cout << "Constructed Player!" << std::endl;
}

IKPlayer::~IKPlayer()
{
    SDL_DestroyTexture(m_Texture);

    m_Map->getSimulation()->DestroyBody(m_Body);
    m_Body = nullptr;
}

void IKPlayer::init()
{
    /* Loading the texture */
    SDL_Surface* surf = IMG_Load(RSC_PLAYER_SPRITE);
    if (surf == NULL)
    {
        std::cout << "Unable to load image " << RSC_PLAYER_SPRITE << "!SDL_image Error : " << IMG_GetError() << std::endl;
        return;
    }
    m_Texture = SDL_CreateTextureFromSurface(m_Map->getRenderer(), surf);
    SDL_FreeSurface(surf);

    m_InAir = false;

    m_Speed = 200.0f * m2p;

    /* Rect */
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.allowSleep = true;
    bd.awake = true;
    bd.fixedRotation = true;
    bd.position = b2Vec2((D_SCREEN_WIDTH / 2) * p2m, -148.0f);

    m_Body = m_Map->getSimulation()->CreateBody(&bd);

    b2PolygonShape shapeRect;
    shapeRect.SetAsBox(m_Width / 2 * p2m, m_Height / 2 * p2m);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shapeRect;
    fixtureDef.density = 3.3f;
    fixtureDef.friction = 0.3f;

    m_Body->CreateFixture(&fixtureDef);

    m_lastYValue = m_Body->GetPosition().y;

    //std::cout << "Starting center: " << m_Body->GetWorldCenter().x << " " << u_b2ToSdl(m_Body->GetWorldCenter().x) << std::endl;
}

void IKPlayer::update(double dt)
{
    b2Vec2 bodyVelocity = m_Body->GetLinearVelocity();

    int xAxis = 0;
    int yAxis = 0;
    if (bodyVelocity.x < 0)
        xAxis = -1;
    else if (bodyVelocity.x > 0)
        xAxis = 1;
    animate(yAxis, xAxis);
}

bool IKPlayer::tryMove()
{
    return false;
}

void IKPlayer::applyVelocity()
{

}

void IKPlayer::render()
{
    SDL_Rect srcRect = {48, 9, 16, 23};     // pixel in the image
    //SDL_Rect dstRect;                       // bounds in screen space 

    /*
    b2Vec2 vertices[4];
    vertices[0] = ((b2PolygonShape*)m_Body->GetFixtureList()->GetShape())->m_vertices[0];   // Bottom right
    vertices[1] = ((b2PolygonShape*)m_Body->GetFixtureList()->GetShape())->m_vertices[1];   // Bottom left
    vertices[2] = ((b2PolygonShape*)m_Body->GetFixtureList()->GetShape())->m_vertices[2];   // Top left
    vertices[3] = ((b2PolygonShape*)m_Body->GetFixtureList()->GetShape())->m_vertices[3];   // Top right 
    */

    float angle = m_Body->GetAngle();

    b2Vec2 center = m_Body->GetWorldCenter();

    center.x = center.x * m2p;
    center.y = center.y * m2p - m_Map->getOffsetY() * m2p;

    m_DstRect.x = center.x - (m_Width / 2);
    m_DstRect.y = u_b2ToSdl(center.y + (m_Height / 2));
    m_DstRect.w = m_Width;
    m_DstRect.h = m_Height;

    SDL_RenderCopyEx(m_Map->getRenderer(), m_Texture, &m_TextureCoords, &m_DstRect, angle, nullptr, SDL_FLIP_NONE);
}

void IKPlayer::walk(Direction dir)
{
    //std::cout << "walking: " << dir << std::endl;
    if (dir == LEFT)
    {
        //m_Body->ApplyForceToCenter(b2Vec2(50.0f * p2m, 1.0f), true);
        m_Body->ApplyLinearImpulseToCenter(b2Vec2(-50.0f * p2m, 1.0f), true);
        //m_Spring->SetMotorSpeed(m_Speed);
    }
    else if (dir == RIGHT)
    {
        m_Body->ApplyLinearImpulseToCenter(b2Vec2(50.0f * p2m, 1.0f), true);
        //m_Spring->SetMotorSpeed(-m_Speed);
    }
    else if (dir == NONE)
    {
        //m_Spring->SetMotorSpeed(0);
    }
}

void IKPlayer::jump()
{
    // Gravity controls how high the player goes and the delta time with which speed

    if (!m_InAir)
    {
        std::cout << "Jumping..." << std::endl;
        //m_VelocityY = -5.0f;
        m_InAir = true;
    }

    if (!m_Body->IsAwake())
    {
        int x = 0;
        int y = 0;
        //SDL_GetMouseState(&x, &y);

        m_Body->ApplyLinearImpulseToCenter(b2Vec2(0, 400.0f), true);
    }
}

void IKPlayer::animate(int y, int x)
{
    int totalFrames = 8;   // Animation besteht jeweils aus 6 sprites
    int delayPerFrame = 130;
    int movingDirection = 0;

    if (x > 0)
        movingDirection = 0; // Anfangsprite ist eins weiter Rechts auf dem Spritesheet
    else
        movingDirection = 8;// Anfangsprite ist 8 weiter Rechts auf dem Spritesheet


    int frame = movingDirection + (SDL_GetTicks() / delayPerFrame) % totalFrames;
    m_TextureCoords.x = frame * m_TextureCoords.w;
    m_TextureCoords.y = 64;
    if (y == 0 && x == 0)
    {
        m_TextureCoords.x = 0 + 32 * ((SDL_GetTicks() / delayPerFrame) % 13);
        m_TextureCoords.y = 0;
        m_TextureCoords.h = 32;
        m_TextureCoords.w = 32;
    }

}