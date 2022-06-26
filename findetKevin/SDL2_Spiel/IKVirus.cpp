#include "IKVirus.h"

#include <SDL_image.h>

#include "Resources.h"
#include "CoordinateConversions.h"

#include "IKRenderLayer.h"
#include "IKPlayer.h"

//#include <cstdlib>
#include <windows.h>

int IKVirus::s_KilledViruses = 0;

IKVirus::IKVirus(IKRenderLayer* layer, SDL_Rect bounds, SDL_Renderer* renderer)
{
	m_ParentLayer = layer;
    m_Renderer = renderer;

    /* Loading the texture */
    SDL_Surface* surf = IMG_Load(RSC_IK_ENEMY_SPRITE);
    if (surf == NULL)
    {
        std::cout << "Unable to load image " << RSC_IK_CHARAKTER_SPRITE << "!SDL_image Error : " << IMG_GetError() << std::endl;
        return;
    }
    m_Texture = SDL_CreateTextureFromSurface(m_ParentLayer->getMap()->getRenderer(), surf);
    SDL_FreeSurface(surf);

    m_Bounds = bounds;

    m_TextureCoords = { 0, 0, 8, 8 };

    m_AnimationSpeed = 60;
    m_FrameCounter = 0;
}

IKVirus::~IKVirus()
{
    SDL_DestroyTexture(m_Texture);
}

void IKVirus::render()
{
    //SDL_Rect dstRect;

    m_DstRect.x = m_Bounds.x;
    m_DstRect.y = m_Bounds.y + m_ParentLayer->getMap()->getOffsetY() * m2p + m_ParentLayer->getPersOffsetY() * m2p + m_ParentLayer->getMap()->getStartingOffsetY() * m2p * m_ParentLayer->getDistance();
    m_DstRect.w = m_Bounds.w;
    m_DstRect.h = m_Bounds.h;

    SDL_RenderCopyEx(m_Renderer, m_Texture, &m_TextureCoords, &m_DstRect, 0, nullptr, SDL_FLIP_NONE);
}

int IKVirus::update()
{
    if (m_FrameCounter == m_AnimationSpeed)
    {
        if (m_TextureCoords.y == 0)
        {
            m_TextureCoords.y = 8;
        }
        else if (m_TextureCoords.y == 8)
        {
            m_TextureCoords.y = 0;
        }

        m_FrameCounter = 0;
    }

    //std::cout << m_TextureCoords.y << " " << m_FrameCounter << std::endl;

    SDL_Rect playerRect = m_ParentLayer->getMap()->getPlayer()->getDstRect();

    if (SDL_HasIntersection(&playerRect, &m_DstRect))
    {
        //std::cout << "g<fsdddddddddddddd" << std::endl;
        gettingKilled();
        IKVirus::s_KilledViruses++;
        return 1;
    }

    m_FrameCounter++;
    return 0;
}

void IKVirus::gettingKilled()
{
    int speed = 500;

    Uint8 r, g, b, a;

    SDL_Rect dstRect;

    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = D_SCREEN_WIDTH;
    dstRect.h = D_SCREEN_WIDTH;
    

    SDL_GetRenderDrawColor(m_Renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_Renderer);
    SDL_RenderFillRect(m_Renderer, &dstRect);

    render();
    SDL_RenderPresent(m_Renderer);

    Sleep(1000);
    
    for (int i = 3; i < 9; i++)
    {
        m_TextureCoords.y = 8 * i;
        SDL_RenderFillRect(m_Renderer, &dstRect);
        render();
        SDL_RenderPresent(m_Renderer);
        Sleep(speed);
    }

    SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
}