#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Player.h"

class Player;

class ExplosionCircle  
{   
    public:
        ExplosionCircle(Player* _owner){}

    private:
        Player* owner;
};

class Explosion : public MovableSprite
{
    public:
        Explosion(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner);
        void SetVelocity(Vec2i v) { velocity = v;}
        void SetLifeTime(int lt) { lifeTime = lt;}
        Player* GetOwner() { return owner; }
        void Tick();
    private:
        int lifeTime = 20;
        Vec2i velocity;
        Player* owner;
};



#endif
