#ifndef SPRITE_H
#define SPRITE_H

#include "GameEngine.h"
#include "SDL2\SDL_image.h"
#include "Vec2i.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include "Constants.h"
#include "Collider2D.h" 

class Sprite
{
    public:
        virtual void Draw() const;
        virtual void Tick() = 0;
        virtual void Move(Vec2i&){}
        virtual ~Sprite();

        const virtual SDL_Rect& GetSrcRect() { return srcRect; }
        const virtual SDL_Rect& GetDestRect() { return destRect; }
        virtual SDL_Texture* GetTexture() {return texture;}
        Collider2D& GetCollider2D();
        std::string GetNameTag() { return nameTag; }

        void InstallCollider2D(SDL_Rect boundsRect, bool bStatic);
        bool HasCollider2D();
        virtual void OnCollision2D(Sprite* other){}
        void SetTag(std::string tag) { nameTag = tag; }
    protected:
        Sprite() {}
        Sprite(Vec2i pos, Vec2i size);
        Sprite(Vec2i pos, Vec2i size, std::string srcImage);

    private:
        SDL_Rect destRect, srcRect;
        SDL_Texture* texture = nullptr;
        Collider2D* collider2D = nullptr;
        std::string nameTag;

};


#endif