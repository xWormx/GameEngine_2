#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include "SDL2\SDL_ttf.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <map>

#include "Level.h"

class GameEngine
{
    public:
        GameEngine(int winWidth, int winHeight, int fps);
        void Run();
        
    private:
        int windowWidth, windowHeight;
        int FramesPerSecond;

        Level* currentLevel;
        
        bool running;
        SDL_Window* window;
        SDL_Renderer* renderer;

        void InitializeSDL();
        void InitializeIMG();
        void InitializeTTF();
        void CreateWindowAndRenderer(int width, int height);


};


#endif