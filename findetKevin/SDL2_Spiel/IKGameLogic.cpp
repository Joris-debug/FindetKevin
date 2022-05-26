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
                    //y_axis = 0;
                    break;

                case SDLK_d:
                case SDLK_a:
                    //x_axis = 0;
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
                    //x_axis = -1;
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

        update(deltaTime);
        render();

        if (deltaTime < float(1000 / 60)) //Limit FPS auf 60
        {
            SDL_Delay(float(1000 / 60) - deltaTime);
        }

        /*
        for (auto cursor : listeVonEntitys)
            cursor->update(0, 0);

        spielerPointer->animation(y_axis, x_axis, deltaTime);   //Neuer Frame für den Player
        collisionDetection(spielerPointer->bewegen(y_axis * deltaTime * 0.225, x_axis * deltaTime * 0.225)); //Neue location für den Player
        SDL_RenderCopy(renderer, currentMap->getTexture(), NULL, currentMap->getPosition());
        spielerPointer->renderer(renderer); // Den Spieler jeden Frame rendern

        NPC_Pathfinding(deltaTime * 0.1);   //alle NPC's werden bewgegt
        for (auto cursor : listeVonEntitys)
        {
            cursor->renderer(renderer);
        }
        SDL_RenderCopy(renderer, currentMap_TopLayer->getTexture(), NULL, currentMap_TopLayer->getPosition());
        */
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