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
    m_Height = 23 * 2;
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
    SDL_Surface* surf = IMG_Load(RSC_IK_CHARAKTER_SPRITE);
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
    bd.position = b2Vec2((D_SCREEN_WIDTH / 2) * p2m, (D_SCREEN_HEIGHT / 2) * p2m);

    m_Body = m_Map->getSimulation()->CreateBody(&bd);

    b2PolygonShape shapeRect;
    shapeRect.SetAsBox(m_Width / 2 * p2m, m_Height / 2 * p2m);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shapeRect;
    fixtureDef.density = 3.0f;
    fixtureDef.friction = 0.3f;

    m_Body->CreateFixture(&fixtureDef);

    /* Wheel */
    /*
    b2CircleShape shapeCircle;
    shapeCircle.m_radius = 46 * p2m;

    fixtureDef.shape = &shapeCircle;

    bd.position = b2Vec2(m_StartingX * p2m, (m_StartingY - 5) * p2m);
    bd.fixedRotation = false;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;
    m_Wheel = m_Map->getSimulation()->CreateBody(&bd);
    m_Wheel->CreateFixture(&fixtureDef);

    float hertz = 4.0f;
    float dampingRatio = .7f;
    float omega = 2.0f * b2_pi * hertz;

    */
    /*
    b2Vec2 axis(0.0f, 1.0f);
    b2WheelJointDef jd;
    jd.Initialize(m_Body, m_Wheel, m_Wheel->GetPosition(), axis);
    jd.motorSpeed = 0.0f;
    jd.maxMotorTorque = 20.0f;
    jd.enableMotor = true;
    jd.stiffness = m_Wheel->GetMass() * omega * omega;
    jd.damping = 2.0f * m_Wheel->GetMass() * dampingRatio * omega;
    jd.lowerTranslation = -0.5f;
    jd.upperTranslation = 0.5f;
    jd.enableLimit = true;
    m_Spring = (b2WheelJoint*)m_Map->getSimulation()->CreateJoint(&jd);

    */

    m_lastYValue = m_Body->GetPosition().y;

    //std::cout << "Starting center: " << m_Body->GetWorldCenter().x << " " << u_b2ToSdl(m_Body->GetWorldCenter().x) << std::endl;
}

void IKPlayer::update(double dt)
{
    dt = dt / 4;

    b2Vec2 bodyPos = m_Body->GetPosition();

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

    SDL_RenderCopyEx(m_Map->getRenderer(), m_Texture, &srcRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);
}

void IKPlayer::walk(Direction dir)
{
    //std::cout << "walking: " << dir << std::endl;
    if (dir == LEFT)
    {
        //m_Body->ApplyForceToCenter(b2Vec2(50.0f * p2m, 1.0f), true);
        m_Body->ApplyLinearImpulseToCenter(b2Vec2(-50.0f * p2m, 1.0f), false);
        //m_Spring->SetMotorSpeed(m_Speed);
    }
    else if (dir == RIGHT)
    {
        m_Body->ApplyLinearImpulseToCenter(b2Vec2(50.0f * p2m, 1.0f), false);
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