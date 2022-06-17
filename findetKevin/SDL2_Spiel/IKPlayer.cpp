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
    m_Inertia = 0.01f;

    m_CollidingLeft = false;
    m_CollidingRight = false;
}

void IKPlayer::update(double dt)
{
    dt = dt / 4;

    m_VelocityX *= dt;
    //m_VelocityY += m_Map->m_Gravity;
    m_VelocityX *= dt;

    if (tryMove())
        applyVelocity();

    m_VelocityX = 0;
    m_VelocityY = 0;
}

bool IKPlayer::tryMove()
{
    Collider temp({ m_Bounds.x + (int) m_VelocityX, m_Bounds.y + (int) m_VelocityY, m_Bounds.w, m_Bounds.h });

    if(checkCollision(&temp) == nullptr)
        return true;

    return false;
}

void IKPlayer::applyVelocity()
{
    m_Bounds.x += m_VelocityX;
    m_Bounds.y += m_VelocityY;
}

void IKPlayer::render()
{
    SDL_Rect srcRect = {48, 9, 16, 23};     // pixel in the image
    SDL_Rect dstRect = m_Bounds;    // bounds in screen space 
    SDL_RenderCopyEx(m_Map->getRenderer(), m_Texture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
}

void IKPlayer::walk(Direction dir)
{
    //std::cout << dir << std::endl;
    m_VelocityX = (dir == LEFT) ? -2.0f : 2.0f;
    m_Dir = dir;
    /*if (dir == LEFT && !m_CollidingLeft)    // Want to go left and is NOT already colliding there, so go for if
    {
        m_VelocityX = -2.0f;
        m_CollidingRight = false;   // Since player moved 
    }*/
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

Collider* IKPlayer::checkCollision(Collider* col)
{
    IKRenderLayer* colLayer = m_Map->getCollisionLayer();
    //Collider playerCollider;
    //playerCollider.m_OffsetY = 0;
    //playerCollider.setRect(&m_Bounds);
    for (auto collider : colLayer->getColliders())
    {
        if (collider->checkCollison(*col))
        {
            return collider;
        }
    }
    return nullptr;
}