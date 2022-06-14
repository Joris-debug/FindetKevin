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

    m_InAir = true;
    m_VelocityX = 0.0f;
    m_VelocityY = 0.0f;
    m_Inertia = 0.01f;

    m_CollidingLeft = false;
    m_CollidingRight = false;
}

void IKPlayer::update(double dt)
{
    dt = dt / 4;

    Collider* collidingObject = checkCollision();
    /* Applying y foce */
    m_Map->setOffsetY(m_Map->getOffsetY() + m_VelocityY * dt * -0.5f);
    if(m_InAir)
        m_VelocityY += m_Map->m_Gravity * dt;

    collidingObject = checkCollision();
    if (collidingObject != nullptr && m_InAir) 
    {
        if (m_VelocityY > 0)    // Player collided while falling on an object (landed)
        {
            m_Map->setOffsetY(m_Bounds.y + m_Bounds.h - collidingObject->getRect().y);
            std::cout << "Player landed" << std::endl;
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
        else
        {
            std::cout << "lol" << std::endl;
        }
    }
    /* Applying x force */
    //std::cout << m_VelocityX * dt * 0.5f << std::endl;
    if(!(m_VelocityX < 0 && m_CollidingLeft) && !(m_VelocityX > 0 && m_CollidingRight))   // Not trying to walk left while already colliding left
        m_Bounds.x += m_VelocityX * dt * 0.5f;


    if(collidingObject != nullptr && !m_CollidingLeft && !m_CollidingRight){     // Collision detected
        if (m_VelocityX < 0)    // Player collided while walking to the left
        {
            m_Bounds.x = collidingObject->getRect().x + collidingObject->getRect().w + 1;
            m_CollidingLeft = true;
        }
        else if (m_VelocityX > 0)   // Player collided while walking to the right 
        {
            std::cout << "Collision detected!" << std::endl;
            m_Bounds.x = collidingObject->getRect().x - m_Bounds.w - 1;
            m_CollidingRight = true;
        }

    }


    if (m_InAir)
    {

    }

    m_VelocityX = 0;

    if (m_Dir == RIGHT)
        m_CollidingLeft = false;

    if (m_Dir == LEFT)
        m_CollidingRight = false;

    m_Dir = NONE;

    //std::cout << m_Bounds.x << " " << m_Bounds.y + m_Map->getOffsetY() << std::endl;
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