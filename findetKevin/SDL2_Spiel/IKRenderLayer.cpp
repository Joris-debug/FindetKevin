#include "IKRenderLayer.h"

#include <SDL_image.h>

#include <iostream>

#include "CoordinateConversions.h"

#include "IKVirus.h"

void IKRenderLayer::init(const std::string& path, SDL_Renderer* renderer, float distance, IKMap* map, int height, float yOffset)
{
    m_Renderer = renderer;
    m_Map = map;
    m_Distance = distance;
    m_PersOffsetY = yOffset;

    SDL_Surface* surf = IMG_Load(path.c_str());
    if (surf == NULL)
    {
        std::cout << "Unable to load image " << path << "!SDL_image Error : " << IMG_GetError() << std::endl;
        return;
    }
    m_Image = SDL_CreateTextureFromSurface(m_Renderer, surf);
    SDL_FreeSurface(surf);

    m_SrcRect.x = 0;
    m_SrcRect.y = 0;
    m_SrcRect.w = 200;
    m_SrcRect.h = height;

    m_DstRect.x = 0;
    m_DstRect.y = m_Map->getOffsetY() + m_PersOffsetY;
    m_DstRect.w = 200*4;
    m_DstRect.h = height*4;
}

void IKRenderLayer::update(double dt)
{
    m_DstRect.y = m_Map->getOffsetY() * m2p * m_Distance + m_PersOffsetY + m_Map->getStartingOffsetY() * m2p;

    //for (auto virus : m_Viruses)
    auto virus = m_Viruses.begin();
    while(virus != m_Viruses.cend())
    {
        if ((*virus)->update() == 1)
        {
            virus = m_Viruses.erase(virus);
        }
        else {
            ++virus;
        }
    }

    for (auto collider : m_Colliders)
    {
        //collider->m_OffsetY = m_Map->getOffsetY();
        collider->update(dt);
    }
}

void IKRenderLayer::render(bool renderCols)
{

    for (auto virus : m_Viruses)
    {
        virus->render();
    }

    SDL_RenderCopy(m_Renderer, m_Image, NULL, &m_DstRect);
            
    if (renderCols)
    {
        for (auto col : m_Colliders)
        {
            col->render(m_Renderer, m_Map->getOffsetY(), m_Map->getStartingOffsetY());
        }
    }
}

IKRenderLayer::~IKRenderLayer()
{
    SDL_DestroyTexture(m_Image);

    m_Colliders.clear();
    m_Viruses.clear();
}
