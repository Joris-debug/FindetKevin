#include "IKGameLogic.h"

#include "IKPlayer.h"

IKGameLogic::IKGameLogic(SDL_Renderer* renderer, SDL_Window* window)
{
	m_Renderer = renderer;
	m_Window = window;
}

void IKGameLogic::init()
{
	SDL_SetRenderDrawColor(m_Renderer, 121, 121, 121, 255);
	SDL_RenderClear(m_Renderer);

    m_Map = new IKMap(m_Renderer);
    m_Map->init();
    m_Player = m_Map->getPlayer();
    m_Map->render();

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
                    m_Player->walk(NONE);
                    break;

                case SDLK_d:
                case SDLK_a:
                    m_Player->walk(LEFT);
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

        /* Gameloop */
        update(deltaTime);
        render();

        /* Restricting fps to 60 */
        if (deltaTime < float(1000 / 60)) //Limit FPS auf 60
        {
            SDL_Delay(float(1000 / 60) - deltaTime);
        }

        SDL_RenderPresent(m_Renderer);
        SDL_RenderClear(m_Renderer);

    }
}

IKGameLogic::~IKGameLogic()
{
    delete m_Map;
}

void IKGameLogic::update(double& dt)
{
    m_Map->update(dt);
}

void IKGameLogic::render()
{
    m_Map->render();
}