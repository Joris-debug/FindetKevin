#include "IKRenderLayer.h"

#include <SDL_image.h>

#include <iostream>

void IKRenderLayer::init(const std::string& path, SDL_Renderer* renderer, float distance, IKMap* map)
{
    m_Renderer = renderer;
    m_Map = map;
    m_Distance = distance;

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
    m_SrcRect.w = 400;
    m_SrcRect.h = 1000;

    m_DstRect.x = 0;
    m_DstRect.y = m_Map->getOffsetY();
    m_DstRect.w = 800;
    m_DstRect.h = 2000;
}

void IKRenderLayer::update(double dt)
{
    m_DstRect.y = m_Map->getOffsetY() * m_Distance;

    for (auto collider : m_Colliders)
    {

        collider->m_OffsetY = m_Map->getOffsetY();
    }
}

void IKRenderLayer::render(bool renderCols)
{

    SDL_RenderCopy(m_Renderer, m_Image, NULL, &m_DstRect);

    if (renderCols)
    {
        for (auto col : m_Colliders)
        {
            col->render(m_Renderer);
        }
    }
}

IKRenderLayer::~IKRenderLayer()
{
    SDL_DestroyTexture(m_Image);

    m_Colliders.clear();
}
