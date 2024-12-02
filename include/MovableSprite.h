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
        ~MovableSprite(){}

    protected:
        MovableSprite(Vec2i p, Vec2i sz, std::string srcImage);
    
    private:
};

#endif