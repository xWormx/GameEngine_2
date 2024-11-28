#include "GameEngine.h"

GameEngine::GameEngine(int winWidth, int winHeight, int fps) : windowWidth(winWidth), windowHeight(winHeight), FramesPerSecond(fps)
{
    InitializeSDL();
    InitializeIMG();
    InitializeTTF();
    CreateWindowAndRenderer(windowWidth, windowHeight);
}

void GameEngine::InitializeSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("InitializeSDL(): Couldn't initialize SDL" + errMsg);
    }
}
void GameEngine::InitializeIMG()
{
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("InitializeIMG(): Couldn't initialize IMG" + errMsg);
    }
}
void GameEngine::InitializeTTF()
{
    if(TTF_Init() < 0)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("InitializeTTF(): Couldn't initialize TTF" + errMsg);
    }
}

void GameEngine::CreateWindowAndRenderer(int width, int height)
{
    window = SDL_CreateWindow("CJ_GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if(!window)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("CreateWindowAndRenderer() Failed to Create a window: " + errMsg);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("CreateWindowAndRenderer() Failed to Create a Renderer: " + errMsg);
    }
        
}

void GameEngine::Run()
{   
    running = true;
    while(running)
    {
        SDL_Event e;
        if(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                {
                    running = false;
                } break;

                case SDL_KEYDOWN:
                {
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                        running = false;
                } break;
            }
        }
    }
}