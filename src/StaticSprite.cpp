#include "StaticSprite.h"

StaticSprite* StaticSprite::GetInstance(Vec2i p, Vec2i sz, std::string srcImage)
{
    return new StaticSprite(p, sz, srcImage);
}

StaticSprite::StaticSprite(Vec2i p, Vec2i sz, std::string srcImage) : Sprite(p, sz, srcImage)
{
}

void StaticSprite::Tick()
{

}

StaticSprite::~StaticSprite()
{
    
}