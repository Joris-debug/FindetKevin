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
        std::cout << "Unable to load image " << RSC_IK_CHARAKTER_SPRITE << "!SDL_image Error : " << IMG_GetError() << std::endl;
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
    dt = dt / 4;

    b2Vec2 bodyPos = m_Body->GetPosition();
    //std::cout << bodyPos.y << std::endl;
    //m_Body->GetLinearVelocity().x
    b2Vec2 bodyVelocity = m_Body->GetLinearVelocity();


    int xAxis = 0;
    int yAxis = 0;
    if (bodyVelocity.x < 0)
        xAxis = -1;
    else if (bodyVelocity.x > 0)
        xAxis = 1;
    std::cout << xAxis << std::endl;
    animate(yAxis, xAxis);

    //m_Bounds = u_B2RectToSdl({(int)bodyPos.x, (int)bodyPos.y, m_Bounds.w, m_Bounds.h});

    //m_Bounds.x = bodyPos.x;
    //m_Bounds.y = bodyPos.y;

    /*
    std::cout << "In SDL space:" << std::endl;
    std::cout << "Player X: " << m_Bounds.x << std::endl;
    std::cout << "Player y: " << m_Bounds.y << std::endl;

    m_Bounds = u_SdlRectToB2(m_Bounds);

    std::cout << "In B2 space:" << std::endl;
    std::cout << "Player X: " << m_Bounds.x << std::endl;
    std::cout << "Player y: " << m_Bounds.y << std::endl;

    m_Bounds = u_B2RectToSdl(m_Bounds);

    std::cout << "In SDL space:" << std::endl;
    std::cout << "Player X: " << m_Bounds.x << std::endl;
    std::cout << "Player y: " << m_Bounds.y << std::endl;

    m_VelocityX *= dt;
    //m_VelocityY += m_Map->m_Gravity;
    m_VelocityX *= dt;
    */

    //if (tryMove())
    //    applyVelocity();

}

bool IKPlayer::tryMove()
{
    /*Collider temp({ m_Bounds.x + (int) m_VelocityX, m_Bounds.y + (int) m_VelocityY, m_Bounds.w, m_Bounds.h }, m_Map->getSimulation());

    if(checkCollision(&temp) == nullptr)
        return true;

    */
    return false;
}

void IKPlayer::applyVelocity()
{

}

void IKPlayer::render()
{
    SDL_Rect srcRect = {48, 9, 16, 23};     // pixel in the image
    SDL_Rect dstRect;                       // bounds in screen space 

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

    //std::cout << "center of player: " << center.x << " " << center.y << std::endl;

    dstRect.x = center.x - (m_Width / 2);
    dstRect.y = u_b2ToSdl(center.y + (m_Height / 2));
    dstRect.w = m_Width;
    dstRect.h = m_Height;

    //std::cout << "y: (normal)" << u_b2ToSdl(center.y + (m_Height / 2)) << " (yOff)" << m_Map->getOffsetY() << " (+)" << u_b2ToSdl(center.y + (m_Height / 2)) + m_Map->getOffsetY() << std::endl;

    SDL_RenderCopyEx(m_Map->getRenderer(), m_Texture, &m_TextureCoords, &dstRect, angle, nullptr, SDL_FLIP_NONE);
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

Collider* IKPlayer::checkCollision()
{
    IKRenderLayer* colLayer = m_Map->getCollisionLayer();
    Collider playerCollider;
    playerCollider.m_OffsetY = 0;
    //playerCollider.setRect(&m_Bounds);
    for (auto collider : colLayer->getColliders())
    {
        //if (collider->checkCollison(playerCollider))
        {
            return collider;
        }
    }
    return nullptr;
}

Collider* IKPlayer::checkCollision(Collider* col)
{
    IKRenderLayer* colLayer = m_Map->getCollisionLayer();
    //Collider playerCollider;
    //playerCollider.m_OffsetY = 0;
    //playerCollider.setRect(&m_Bounds);
    for (auto collider : colLayer->getColliders())
    {
        //if (collider->checkCollison(*col))
        {
            return collider;
        }
    }
    return nullptr;
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