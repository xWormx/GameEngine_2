#include "Explosion.h"

Explosion::Explosion(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner) : MovableSprite(p, sz, srcImage), owner(_owner) 
{
}

void Explosion::Tick()
{
    if(lifeTime-- < 0)
    {
        gameEngine.PlaySound("explosion", 10);
        gameEngine.GetCurrentLevel()->RemoveSprite(this);
    }
    else
    {
        Move(velocity);
    }
}