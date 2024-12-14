#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include "SDL2\SDL_ttf.h"
#include "SDL2\SDL_mixer.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <random>
#include "Vec2i.h"
#include "Level.h"

class Level;
class Sprite;

class GameEngine
{
    public:
        GameEngine(int winWidth, int winHeight);
        void Run();
        void SetFps(unsigned int fps);

        void RegisterKeyCallback(SDL_Keycode key, void(*callback)())
        {
            keyCallbacks[key].push_back(callback);
        }
        
        template <typename T>
        void RegisterKeyCallback(SDL_Keycode key, T* instance, void(T::*callback)())
        {
            auto wrapper = [instance, callback]()
            {
                (instance->*callback)();
            };
            memberKeyCallbacks[key].push_back(wrapper);
        }

        void AddLevel(Level* level);
        void DrawLevel();
        void LoadLevel(unsigned int levelIndex);
        void SetCurrentLevel();
        const unsigned int GetCurrentLevelIndex();
        Level* GetCurrentLevel() {return currentLevel;}

        void LoadSound(std::string nameKey, std::string srcPath);
        void PlaySound(std::string keyName, int volume);

        void LoadMusic(std::string nameKey, std::string srcPath);
        void PlayMusic(std::string keyName, int volume);

        int GetRandomNumberInRange(int min, int max);
        SDL_Renderer* GetRenderer() {return renderer;}
        TTF_Font* GetFont()         {return font;}

        ~GameEngine();

    private:
        int windowWidth, windowHeight;
        int framesPerSecond, tickInterval;

        std::random_device randomDevice;
        std::mt19937 gen;

        std::string strTextInput = "";
        bool textInputRecieved;

        bool loadLevelRequested;
        unsigned int levelIndexToLoad;
        Level* currentLevel;
        std::vector<Level*> levels;

        bool running;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;

        std::unordered_map<SDL_Keycode, std::vector<void(*)()>> keyCallbacks;
        std::unordered_map<SDL_Keycode, std::vector<std::function<void()>>> memberKeyCallbacks;

        TTF_Font* font;

        std::unordered_map<std::string, Mix_Chunk*> soundMap;
        std::unordered_map<std::string, Mix_Music*> musicMap;

        void SetupRandomGenerator();
        void InitializeSDL();
        void InitializeIMG();
        void InitializeTTF();
        void InitializeMixer();
        void CreateWindowAndRenderer(int width, int height);
        void HandleEvents();
        void HandleKeyCallbacks(const SDL_Keycode& key);
        void HandleCollisions();
        bool CheckCollision(Sprite* s, Sprite* other);
        void ResolveCollision(Sprite* s, Sprite* other);
        void AdjustTickInterval() { tickInterval = 1000 / framesPerSecond; }

        void DEBUGDrawColliders();
};

extern GameEngine gameEngine;

#endif