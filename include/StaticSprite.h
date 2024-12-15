#ifndef STATICSPRITE_H
#define STATICSPRITE_H

#include "Sprite.h"
#include "Vec2i.h"

class StaticSprite : public Sprite
{
    public:
        static StaticSprite* GetInstance(Vec2i p, Vec2i sz, std::string srcImage);
        void Tick();
        ~StaticSprite();

    protected:
        StaticSprite(Vec2i p, Vec2i sz, std::string srcImage);
};


#endif