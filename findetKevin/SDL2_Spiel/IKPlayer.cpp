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
    dt = dt / 4;

    m_Bounds.x += m_VelocityX * dt * 0.5;
    Collider* collidingObject = checkCollision();
    if(collidingObject != nullptr){
        m_Bounds.x -= m_VelocityX * dt * 0.5;
    }

    m_Map->setOffsetY(m_Map->getOffsetY() + m_VelocityY * dt * -0.5);
    collidingObject = checkCollision();
    if (collidingObject != nullptr && m_InAir) 
    {
        if (m_VelocityY > 0)    // Player collided while falling on an object (landed)
        {
            m_Map->setOffsetY(m_Bounds.y + m_Bounds.h - collidingObject->getRect().y);
            m_VelocityY = 0;
            m_InAir = false;
        }
        else if (m_VelocityY < 0)   // Player collided while jumping
        {
            std::cout << "lol ------------------------------------------------------------------------------------------------" << std::endl;
            //m_Map->setOffsetY(collidingObject->getRect().y - collidingObject->getRect().h);
            m_Map->setOffsetY(m_Bounds.y - collidingObject->getRect().y - collidingObject->getRect().h);
            m_VelocityY = m_VelocityY * (-1);   // Hits his head and falls back down immediatly
        }
    }

    if (m_InAir)
    {
        m_VelocityY += m_Map->m_Gravity * dt;
    }
}

void IKPlayer::render()
{
    SDL_Rect srcRect = {48, 9, 16, 23};     // pixel in the image
    SDL_Rect dstRect = m_Bounds;    // bounds in screen space 
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
    playerCollider.m_OffsetY = 0;
    playerCollider.setRect(&m_Bounds);
    for (auto collider : colLayer->getColliders())
    {
        if (collider->checkCollison(playerCollider))
        {
            return collider;
        }
    }
    return nullptr;
}