#include "IKPlayer.h"

#include "SDL_image.h"

#include <iostream>

#include "Resources.h"

IKPlayer::IKPlayer(IKMap* map)
{
    m_Bounds.x = 800 / 2 + 8;
    m_Bounds.y = 620 / 2; 
    m_Bounds.w = 16 * 2;
    m_Bounds.h = 32 * 2;

    m_Map = map;
}

void IKPlayer::render()
{
    SDL_Rect srcRect = {48, 9, 16, 23};
    SDL_Rect dstRect = {400, 340, 16, 23};
    std::cout << "Rendering the IKPlayer!" << std::endl;
    SDL_RenderCopyEx(m_Map->getRenderer(), m_Texture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
}

void IKPlayer::walk(Direction dir)
{

}

void IKPlayer::init()
{
    SDL_Surface* surf = IMG_Load(RSC_IK_CHARAKTER_SPRITE);

    if (surf == NULL)
    {
        std::cout << "Unable to load image " << RSC_IK_CHARAKTER_SPRITE << "!SDL_image Error : " << IMG_GetError() << std::endl;
        //ERROR("Unable to load image (\"{}\")! SDL_image Error: {}", path, IMG_GetError());
        return;
    }
    m_Texture = SDL_CreateTextureFromSurface(m_Map->getRenderer(), surf);
}