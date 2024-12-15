#include "GameEngine.h"

// Sätt till 0 om colliderbounds inte ska ritas ut
#define DEBUGDRAW 1

extern GameEngine gameEngine(1280, 920);

GameEngine::GameEngine(int winWidth, int winHeight) : windowWidth(winWidth), windowHeight(winHeight), framesPerSecond(60)
{
    InitializeSDL();
    InitializeIMG();
    InitializeTTF();
    InitializeMixer();
    CreateWindowAndRenderer(windowWidth, windowHeight);
}

void GameEngine::Run()
{   
    running = true;
    SDL_StartTextInput();

    while(running)
    {
        Uint32 nextTick = SDL_GetTicks() + tickInterval;
        textInputRecieved = false;
        strTextInput = "";
        inputComponent.ClearSingleInputKeys();

        if(loadLevelRequested)
            SetCurrentLevel();
             
        
        HandleEvents();
        
        /* Sortera sprites */

        currentLevel->Update(*this);
        HandleCollisions();

        SDL_RenderClear(renderer);
        
        DrawLevel(); 

        /* Delay */
        int delay = nextTick - SDL_GetTicks();
        if(delay > 0)
            SDL_Delay(delay);

    }

    SDL_StopTextInput();
}

void GameEngine::SetFps(unsigned int fps)
{
    framesPerSecond = fps;
    AdjustTickInterval();
}

void GameEngine::HandleEvents()
{
    if(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                {
                    running = false;
                } break;
            case SDL_KEYDOWN:
                {
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        running = false;

                    HandleKeyDownEvents(event);
                    HandleKeyCallbacks(event.key.keysym.sym);

                } break;
            case SDL_KEYUP:
                {
                    HandleKeyUpEvents(event);
                } break;
            
            case SDL_MOUSEBUTTONDOWN:
                {
                    HandleMouseDownEvents(event);
                } break;
            
            case SDL_MOUSEBUTTONUP:
                {
                    HandleMouseUpEvents(event);
                } break;
            case SDL_TEXTINPUT:
                {
                    textInputRecieved = true;
                    strTextInput += event.text.text;
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

void GameEngine::HandleCollisions()
{
    for(Sprite* s : currentLevel->GetColliderSprites())
    {
        for(Sprite* other : currentLevel->GetColliderSprites())
        {
            if(!s->GetCollider2D().IsStatic() && !other->GetCollider2D().IsStatic())
            {
                if(s != other)
                {
                    if(CheckCollision(s, other))
                    {
                        ResolveCollision(s, other);
                        s->OnCollision2D(other);
                    }
                }     
            }
        }     
    }
}

bool GameEngine::CheckCollision(Sprite* s, Sprite* other)
{
    SDL_Rect rectA = s->GetCollider2D().GetBounds();
    SDL_Rect rectB = other->GetCollider2D().GetBounds();

    if((rectA.x + rectA.w) > rectB.x &&
        rectA.x < (rectB.x + rectB.w))
    {
        if((rectA.y + rectA.h) > rectB.y &&
            rectA.y < (rectB.y + rectB.h))
                return true;
    }

    return false;
}

void GameEngine::ResolveCollision(Sprite* s, Sprite* other)
{
    SDL_Rect sRect = s->GetDestRect();
    SDL_Rect otherRect = other->GetDestRect();

    int left, right, top, bottom;
    
    if(sRect.x < otherRect.x)
    {
        right = sRect.x + sRect.w;
        left = otherRect.x;
    }
    else
    {
        right = otherRect.x + otherRect.w;
        left = sRect.x;
    }

    if(sRect.y < otherRect.y)
    {
        top = otherRect.y;
        bottom = sRect.y + sRect.h;
    }
    else
    {
        top = sRect.y;
        bottom = otherRect.y + otherRect.h;
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

void GameEngine::DrawLevel()
{
    for(Sprite* s : currentLevel->GetSprites())
    {
        s->Draw();
    }  

#if DEBUGDRAW
    DEBUGDrawColliders();
#endif

    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xff);
    SDL_RenderPresent(renderer);

}

void GameEngine::DEBUGDrawColliders()
{
    for(Sprite* s : currentLevel->GetColliderSprites())
    {
        bool didCollide = false;
        for(Sprite* other : currentLevel->GetColliderSprites())
        {
            if(!s->GetCollider2D().IsStatic() && !other->GetCollider2D().IsStatic())
            {
                if(s != other)
                {
                    if(CheckCollision(s, other))
                    {
                        didCollide = true;
                    }
                }     
            }
        }     

        if(didCollide)
            SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(GetRenderer(), 0, 255, 0, 255);        
        
        SDL_RenderDrawRect(GetRenderer(), &s->GetCollider2D().GetBounds());
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

const unsigned int GameEngine::GetCurrentLevelIndex()
{
    return currentLevel->GetLevelIndex();
}

void GameEngine::SetupRandomGenerator()
{
    gen = std::mt19937(randomDevice());
}

int GameEngine::GetRandomNumberInRange(int min, int max)
{
    std::uniform_int_distribution<> distr(min, max);

    return distr(gen);
}

const Vec2i GameEngine::GetWindowSize()
{
    Vec2i size = {};

    SDL_GetWindowSize(window, &size.x, &size.y);

    return size;
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

    std::string fontFile = constants::gResPath + "\\fonts\\coure.fon";
    int fontSize = 48;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if(!font)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("InitializeTTF(): Couldn't Open font " + fontFile + ": " + errMsg);
    }
}

void GameEngine::InitializeMixer()
{
    
    int result = Mix_OpenAudio(20050, AUDIO_S16SYS, 2, 96);
    std::string errMsg = Mix_GetError();
    if(result != 0)
        throw std::runtime_error("InitializeMixer(): Failed to Open Audio " + errMsg);
    
    Mix_AllocateChannels(128);
}


void GameEngine::LoadSound(std::string nameKey, std::string srcPath)
{
    if(soundMap.find(nameKey) != soundMap.end()) 
        throw std::runtime_error(nameKey + " is alreay an existing sound, name it something different!");

    std::string fullSrcPath = constants::gResPath + "sounds\\" + srcPath; 
    Mix_Chunk* sound = Mix_LoadWAV(fullSrcPath.c_str());

    std::string errMsg = Mix_GetError();
    if(!sound)  
        throw std::runtime_error("InitializeMixer(): Couldn't load WAV, " + errMsg);

    soundMap[nameKey] = sound;
}



void GameEngine::PlaySound(std::string keyName, int volume)
{
    Mix_VolumeChunk(soundMap.at(keyName), volume);
    Mix_PlayChannel(-1, soundMap.at(keyName), 0);
}

void GameEngine::LoadMusic(std::string nameKey, std::string srcPath)
{
    if(musicMap.find(nameKey) != musicMap.end()) 
        throw std::runtime_error(nameKey + " is alreay an existing sound, name it something different!");

    std::string fullSrcPath = constants::gResPath + "sounds\\" + srcPath; 
    Mix_Music* music = Mix_LoadMUS(fullSrcPath.c_str());

    std::string errMsg = Mix_GetError();
    if(!music)  
        throw std::runtime_error("InitializeMixer(): Couldn't load Music, " + errMsg);

    musicMap[nameKey] = music;
}

void GameEngine::PlayMusic(std::string keyName, int volume)
{
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(musicMap.at(keyName), -1);
}

void GameEngine::HandleKeyDownEvents(const SDL_Event& event)
{
    inputComponent.SetKeyCodePressed(inputComponent.GetKeyCodeFromEvent(event));
}

void GameEngine::HandleKeyUpEvents(const SDL_Event& event)
{
    inputComponent.SetKeyCodeReleased(inputComponent.GetKeyCodeFromEvent(event)); 
}

void GameEngine::HandleMouseDownEvents(const SDL_Event& event)
{
    
    inputComponent.SetMousePressed(inputComponent.GetMouseButtonFromEvent(event));
}

void GameEngine::HandleMouseUpEvents(const SDL_Event& event)
{
    
    inputComponent.SetMouseReleased(inputComponent.GetMouseButtonFromEvent(event));
        
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

GameEngine::~GameEngine()
{
    for(std::pair p : soundMap)
    {
        
        if(p.second)
            Mix_FreeChunk(p.second);
    }
    
    for(std::pair p : musicMap)
    {
        if(p.second)
            Mix_FreeMusic(p.second);
    }

    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    SDL_Quit();
}

