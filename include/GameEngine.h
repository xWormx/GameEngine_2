#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include "SDL2\SDL_ttf.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "Vec2i.h"
#include "Level.h"

class Level;
class Sprite;

class GameEngine
{
    public:
        GameEngine(int winWidth, int winHeight);
        void Run();
        void SetFps(unsigned int fps) { framesPerSecond = fps; }

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
        void DrawLevel() const;
        void LoadLevel(unsigned int levelIndex);
        void SetCurrentLevel();

        SDL_Renderer* GetRenderer() {return renderer;}
        TTF_Font* GetFont()         {return font;}
    private:
        int windowWidth, windowHeight;
        int framesPerSecond;

        bool loadLevelRequested;
        unsigned int levelIndexToLoad;
        Level* currentLevel;
        std::vector<Level*> levels;

        bool running;
        SDL_Window* window;
        SDL_Renderer* renderer;

        std::unordered_map<SDL_Keycode, std::vector<void(*)()>> keyCallbacks;
        std::unordered_map<SDL_Keycode, std::vector<std::function<void()>>> memberKeyCallbacks;

        TTF_Font* font;
        void InitializeSDL();
        void InitializeIMG();
        void InitializeTTF();
        void CreateWindowAndRenderer(int width, int height);
        void HandleEvents();
        void HandleKeyCallbacks(const SDL_Keycode& key);
        void HandleCollisions();
        bool CheckCollision(Sprite* s, Sprite* other);
        void ResolveCollision(Sprite* s, Sprite* other);
};

extern GameEngine gameEngine;

#endif