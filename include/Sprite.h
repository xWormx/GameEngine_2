#ifndef SPRITE_H
#define SPRITE_H

#include "GameEngine.h"
#include "SDL2\SDL_image.h"
#include "Vec2i.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include "Constants.h"

class Sprite
{
    public:
        virtual void Draw() const;
        virtual void Tick() = 0;
        virtual void OnKeyDown() {}
        virtual void OnKeyUp() {}
        virtual void OnMouseUp() {}
        virtual void OnMouseDown() {}
        virtual ~Sprite();

    protected:
        Sprite(Vec2i pos, Vec2i size, std::string srcImage);

    private:
        SDL_Rect destRect, srcRect;
        SDL_Texture* texture;

};


#endif