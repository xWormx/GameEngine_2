#include "MovableSprite.h"

MovableSprite* MovableSprite::GetInstance(Vec2i p, Vec2i sz, std::string srcImage)
{
    return new MovableSprite(p, sz, srcImage);
}

MovableSprite::MovableSprite(Vec2i p, Vec2i sz, std::string srcImage) : Sprite(p, sz, srcImage)
{

}

void MovableSprite::Draw() const
{

}

void MovableSprite::Tick()
{

}