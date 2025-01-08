#ifndef BULLET_H
#define BULLET_H

#include "MovableSprite.h"
#include "Player.h"
#include "Explosion.h"

class Player;

class Bullet : public MovableSprite
{
    public:
        Bullet(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner);
        void Tick();
        void SpawnExplosion();
        void OnCollision2D(Sprite* other);

        Player* GetOwner() {return owner;}
    private:
        int lifeTime = 40;
        Player* owner;
        Vec2i velocity;
};

#endif