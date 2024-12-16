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
#include "InputComponent.h"

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

        void SetWindowSize(int w, int h) { SDL_SetWindowSize(window, w,h); }
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

        void AppendTextInput(std::string& text) { text += strTextInput;}
        bool GetTextInputRecieved() {return textInputRecieved;}

        int GetRandomNumberInRange(int min, int max);

        bool GetKeyPressedOnce(const int keyCode) const { return inputComponent.GetKeyPressedOnce(keyCode); }
        bool GetKeyPressed(const int keyCode) const { return inputComponent.GetKeyPressed(keyCode); }
        bool GetMousePressed(const int keyCode) const { return inputComponent.GetMousePressed(keyCode); }

        const Vec2i GetWindowSize();
        const Vec2i GetMousePosition();

        SDL_Renderer* GetRenderer() {return renderer;}
        TTF_Font* GetFont()         {return font;}

        ~GameEngine();

    private:
        int windowWidth, windowHeight;
        int framesPerSecond, tickInterval;

        bool running;
        
        SDL_Window* window;
        SDL_Renderer* renderer;
        TTF_Font* font;
        SDL_Event event;
        
        std::random_device randomDevice;
        std::mt19937 gen;

        std::string strTextInput = "";
        bool textInputRecieved;

        InputComponent inputComponent;

        bool loadLevelRequested;
        unsigned int levelIndexToLoad;
        Level* currentLevel;
        std::vector<Level*> levels;

        std::unordered_map<SDL_Keycode, std::vector<void(*)()>> keyCallbacks;
        std::unordered_map<SDL_Keycode, std::vector<std::function<void()>>> memberKeyCallbacks;



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
        
        void HandleKeyDownEvents(const SDL_Event&);
        void HandleKeyUpEvents(const SDL_Event&);
        void HandleMouseDownEvents(const SDL_Event&);
        void HandleMouseUpEvents(const SDL_Event&);

        void DEBUGDrawColliders();
};

extern GameEngine gameEngine;

#endif