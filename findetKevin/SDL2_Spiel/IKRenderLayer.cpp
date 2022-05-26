#include "IKRenderLayer.h"

#include <SDL_image.h>

#include <iostream>

void IKRenderLayer::init(const std::string& path, SDL_Renderer* renderer, float distance)
{
    m_Renderer = renderer;

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
    m_DstRect.y = 0;
    m_DstRect.w = 800;
    m_DstRect.h = 2000;

    //std::cout << "lol" << std::endl;
}

void IKRenderLayer::update(double dt)
{
    //std::cout << "Updating the Background layer! " << m_DstRect.y << " " << dt << std::endl;
    //m_DstRect.y = m_DstRect.y + 0.1f * dt;
}

void IKRenderLayer::render(bool renderCols)
{
    //std::cout << "Rendering the Background layer!" << std::endl;
    //SDL_Rect dstRect{ 0, 20, 800, 2000};
    //m_DstRect.y += 20;
    //std::cout << m_DstRect.y << std::endl;
    //m_DstRect.y += 20;
    //std::cout << "Rendering with: " << m_DstRect.y << std::endl;
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