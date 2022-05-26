#include "IKPlayer.h"

#include "SDL_image.h"

#include "Collider.h"
#include "IKRenderLayer.h"

#include <iostream>
#include <list>

#include "Resources.h"

IKPlayer::IKPlayer(IKMap* map)
{
    m_Bounds.w = 16 * 2;
    m_Bounds.h = 23 * 2;
    m_Bounds.x = 800 / 2 - (m_Bounds.w / 2)+10;
    m_Bounds.y = 620 / 2 - (m_Bounds.h / 2);

    m_Map = map;

    std::cout << "Constructed Player!" << std::endl;
}

IKPlayer::~IKPlayer()
{
    SDL_DestroyTexture(m_Texture);
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
    m_VelocityX = 0.0f;
    m_VelocityY = 0.0f;
}

void IKPlayer::update(double dt)
{
    dt = dt / 3;

    m_Bounds.x += m_VelocityX * dt * 0.5;       // Apply force on x axis
    Collider* collidingObject = checkCollision();
    if(collidingObject != nullptr){                        // check for collision
        m_Bounds.x -= m_VelocityX * dt * 0.5;       // take back the applyed force if it leads to collision
        //std::cout << "Collision detected on x axis" << std::endl;
    }

    m_Bounds.y += m_VelocityY * dt * 0.5;
    collidingObject = checkCollision();
    if (collidingObject != nullptr && m_InAir) 
    {
        if (m_VelocityY > 0)    // Player collided while falling on an object (landed)
        {
            m_Bounds.y = collidingObject->getRect().y - m_Bounds.h;
            m_VelocityY = 0;
            m_InAir = false;
            std::cout << "hftfjfkghghtjhjjdhgtrjzrjjzrztzkjgdghdjsjjshjhjhsfgsfhdjbgsb10 " << std::endl;
        }
        else if (m_VelocityY < 0)   // Player collided while jumping
        {
            std::cout << "-----------------------------------------------------------------" << std::endl;
            m_Bounds.y = collidingObject->getRect().y + collidingObject->getRect().h;
            m_VelocityY = m_VelocityY * (-1);   // Hits his head and falls back down immediatly
        }
    }

    if (m_InAir)
    {
        m_VelocityY += m_Map->m_Gravity * dt;
    }

    std::cout << "y pos " << m_Bounds.y << std::endl;
    std::cout << "y vel " << m_VelocityY << std::endl;
}

void IKPlayer::render()
{
    SDL_Rect srcRect = {48, 9, 16, 23};     // pixel in the image
    //SDL_Rect dstRect = {400, 340, 16*2, 23*2};
    SDL_Rect dstRect = m_Bounds;    // bounds in screen space 
    //std::cout << "Rendering the IKPlayer!" << std::endl;
    SDL_RenderCopyEx(m_Map->getRenderer(), m_Texture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
}

void IKPlayer::walk(Direction dir)
{

}

void IKPlayer::jump()
{
    // Gravity controls how high the player goes and the delta time with which speed

    if (!m_InAir)
    {
        std::cout << "Jumping..." << std::endl;
        m_VelocityY = -5.0f;
        m_InAir = true;
    }
}

Collider* IKPlayer::checkCollision()
{
    IKRenderLayer* colLayer = m_Map->getCollisionLayer();
    Collider playerCollider;
    playerCollider.setRect(&m_Bounds);
    for (auto collider : colLayer->getColliders())
    {
        if (collider->checkCollison(playerCollider))
        {
            //std::cout << "Collision detected" << std::endl;
            return collider;
        }
    }
    return nullptr;
}