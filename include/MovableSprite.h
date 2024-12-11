#ifndef MOVABLESPRITE_H
#define MOVABLESPRITE_H

#include "Sprite.h"

class MovableSprite : public Sprite
{
    public:
        static MovableSprite* GetInstance(Vec2i p, Vec2i sz, std::string srcImage);
        void Draw() const;
        void Tick();
        void Print() { std::cout << "FROM MOV SPRITE\n"; }
        const SDL_Rect& GetDestRect() {return movableDestRect; }
        const SDL_Rect& GetSrcRect() {return movableSrcRect; }
        void Move(Vec2i movement) 
        { 
            movableDestRect.x += movement.x;
            movableDestRect.y += movement.y;
            if(HasCollider2D())
            {
                    
            } 
        }
        /* void UpdateRgdb(); */
        ~MovableSprite();

    protected:
        MovableSprite(Vec2i p, Vec2i sz, std::string srcImage);
    
    private:
        SDL_Rect movableDestRect, movableSrcRect;
        SDL_Texture* movableTexture = nullptr;
};

#endif