#include "IKGameLogic.h"

#include "IKPlayer.h"

#include "IKVirus.h"

#include "CSavefile.h"

#include "Resources.h"

#include <SDL_image.h>

IKGameLogic::IKGameLogic(SDL_Renderer* renderer, SDL_Window* window, CSavefile* savefile)
{
	m_Renderer = renderer;
	m_Window = window;
    m_GameBeat = false;
    m_ShowEndingScreen = false;
    m_Savefile = savefile;
}

void IKGameLogic::init()
{
	SDL_SetRenderDrawColor(m_Renderer, 121, 121, 121, 255);
	SDL_RenderClear(m_Renderer);

    m_Map = new IKMap(m_Renderer);
    m_Map->init();
    m_Player = m_Map->getPlayer();
    m_Map->render();

    SDL_Surface* surf = IMG_Load(RSC_ENDING_SCREEN);
    if (surf == NULL)
    {
        std::cout << "Unable to load image " << RSC_ENDING_SCREEN << "!SDL_image Error : " << IMG_GetError() << std::endl;
        return;
    }
    m_EndingScreen = SDL_CreateTextureFromSurface(m_Map->getRenderer(), surf);
    SDL_FreeSurface(surf);

	SDL_RenderPresent(m_Renderer);

    bool quit = false;
    Uint32 currentTime = SDL_GetTicks(); //Zum errechnen der Deltatime
    while (!quit)
    {
        Uint32 lastTime = currentTime;
        currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime);
        while (SDL_PollEvent(&m_Event) > 0)
        {
            switch (m_Event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYUP:

                switch (m_Event.key.keysym.sym)
                {

                case SDLK_w:
                case SDLK_s:
                    //m_Player->walk(NONE);
                    break;

                case SDLK_d:
                case SDLK_a:
                    //m_Player->walk(LEFT);
                    break;
                case SDLK_SPACE:
                    //m_Player->jump();
                    break;
                }


            case SDL_KEYDOWN:

                switch (m_Event.key.keysym.sym)
                {

                case SDLK_w:
                    //y_axis = -1;
                    break;
                case SDLK_s:
                    //y_axis = 1;
                    break;

                case SDLK_a:
                    //std::cout << "lol" << std::endl;
                    //m_Player->walk(LEFT);
                    break;
                case SDLK_d:
                    //x_axis = 1;
                    break;
                case SDLK_SPACE:
                    m_Player->jump();
                    break;
                }

                break;
            }
        }


        /* Checks for continues inputs */
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_A])
        {
            m_Player->walk(LEFT);
        }
        else if (keystates[SDL_SCANCODE_D])
        {
            m_Player->walk(RIGHT);
        }
        else if (keystates[SDL_SCANCODE_SPACE] && m_ShowEndingScreen)
        {
            m_ShowEndingScreen = false;
            m_GameBeat = true;
        }

        /* Gameloop */
        if (!m_ShowEndingScreen)
        {
            update(deltaTime);
            render();
        }
        else {
            renderEndingScreen();
        }

        /* Restricting fps to 60 */
        if (deltaTime < float(1000 / 60)) //Limit FPS auf 60
        {
            SDL_Delay(float(1000 / 60) - deltaTime);
        }

        SDL_RenderPresent(m_Renderer);
        SDL_RenderClear(m_Renderer);

        if (m_GameBeat)
        {
            return;
        }
    }

}

IKGameLogic::~IKGameLogic()
{
    delete m_Map;
    *(m_Savefile->getLevel()) = 4;
    IKVirus::s_KilledViruses = 0;
    SDL_DestroyTexture(m_EndingScreen);
}

void IKGameLogic::update(double& dt)
{
    m_Map->update(dt);
    if (IKVirus::s_KilledViruses == 3)
    {
        std::cout << "Congrats! You beat the game!" << std::endl;
        m_ShowEndingScreen = true;
    }
}

void IKGameLogic::render()
{
    m_Map->render();
}

void IKGameLogic::renderEndingScreen()
{
    SDL_Rect dstRect = { 0, 0, D_SCREEN_WIDTH, D_SCREEN_HEIGHT };
    //SDL_Rect srcRect = { 0, 0, D_SC }

    SDL_RenderCopy(m_Renderer, m_EndingScreen, &dstRect, &dstRect);
}