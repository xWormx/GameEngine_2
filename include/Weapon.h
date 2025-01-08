#ifndef WEAPON_H
#define WEAPON_H

#include "Explosion.h" // Make sure this comes first to have access to the Explosion class
#include "Player.h"      // Then include Player.h
#include "Bullet.h"

class Bullet;

class Weapon : public MovableSprite
{
    public:
        Weapon(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner);

        Player* GetOwner() { return owner; }
        void Shoot();
        void Tick();

    private:
        Player* owner;
        Bullet* activeBullet;
        
};

#endif