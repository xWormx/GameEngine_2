#include "GameEngine.h"

extern GameEngine gameEngine(600, 400);

GameEngine::GameEngine(int winWidth, int winHeight) : windowWidth(600), windowHeight(400), framesPerSecond(60)
{
    InitializeSDL();
    InitializeIMG();
    InitializeTTF();
    CreateWindowAndRenderer(windowWidth, windowHeight);
}

void GameEngine::Run()
{   
    running = true;
    /* Handle framrate */
    /* start text input */
    while(running)
    {
        if(loadLevelRequested)
            SetCurrentLevel();
             
        
        HandleEvents();
        
        /* Sortera sprites */

        currentLevel->Update(*this);

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xaa, 0x11, 0xff);
        
        DrawLevel(); 
       
        SDL_RenderPresent(renderer);

        /* Delay */
    }

    /* Stop textinput */
}

void GameEngine::HandleEvents()
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
                HandleKeyCallbacks(e.key.keysym.sym);
            } break;
        }
    }
}

void GameEngine::HandleKeyCallbacks(const SDL_Keycode& key)
{
    if(keyCallbacks.count(key))
    {
        for(auto callback : keyCallbacks[key])
            callback();
    }
    
    if(memberKeyCallbacks.count(key))
    {
        for(auto callback : memberKeyCallbacks[key])
            callback();
    }
}

void GameEngine::AddLevel(Level* level)
{
    if(!level)
        throw std::runtime_error("Level was not valid");
    
    for(Level* l : levels)
    {
        if(l->GetLevelIndex() == level->GetLevelIndex())
        {
            std::string errMsg = "There's already a level with levelIndex" + level->GetLevelIndex();
            throw std::runtime_error(errMsg);
        }  
    }

    levels.push_back(level);

    if(levels.size() == 1)
    {
        LoadLevel(level->GetLevelIndex());
        SetCurrentLevel();
    }


}
void GameEngine::DrawLevel() const
{
    for(Sprite* s : currentLevel->GetSprites())
    {
        s->Draw();
    }    
}

void GameEngine::LoadLevel(unsigned int levelIndex)
{
    loadLevelRequested = true;
    levelIndexToLoad = levelIndex;

}

void GameEngine::SetCurrentLevel()
{
    for(Level* l : levels)
    {
        if(l->GetLevelIndex() == levelIndexToLoad)
            currentLevel = l;
    }
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

