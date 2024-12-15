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
        void SetSpriteRegion(Vec2i p, Vec2i sz);
        int AnimateSprite(Vec2i frameStart, Vec2i frameSize, unsigned int maxFrames, unsigned int animSpeed);
        
        virtual void Move(Vec2i& movement); 
        /* void UpdateRgdb(); */
        ~MovableSprite();

    protected:
        MovableSprite(){}
        MovableSprite(Vec2i p, Vec2i sz, std::string srcImage);
    
    private:
        SDL_Rect movableDestRect, movableSrcRect;
        SDL_Texture* movableTexture = nullptr;
        
        Vec2i lastAnimationFrameStart = {};
        unsigned int animationFrame = 0;
        unsigned int animationTick = 0;

};

#endif