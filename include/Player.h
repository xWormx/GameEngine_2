#ifndef PLAYER_H
#define PLAYER_H

#include "GameEngine.h"
#include "HealthBar.h"
#include "Weapon.h"

class Weapon;

class Player : public MovableSprite
{
    public:
        Player(Vec2i p, Vec2i sz, std::string srcImage, TextFragment* tfName, TextFragment* _tfScore, int health);
        
        TextFragment* GetName() { return name; }
        int GetPoints() { return iPoints; }
        Vec2i GetSpeed() { return speed; }
        void IncreasePoints(int points) { iPoints += points; pointIncreased = true;}
        void UpdatePoints()
        {
            tfScore->SetText(name->GetText() + " " + std::to_string(iPoints));
            pointIncreased = false;
        }

        void Tick();
        void SetMovementKeys(int l, int r, int u, int d) { leftKey = l; rightKey = r; upKey = u; downKey = d;}
        void SetCurrentWeapon(Weapon* weapon) {currentWeapon = weapon;}
        void OnCollision2D(Sprite* other)
        {

        }

    private:
        Vec2i speed = {};
        TextFragment* name;
        TextFragment* tfScore;
        bool shooting = false;
        int leftKey, rightKey, upKey, downKey;
        HealthBar* healthBar;
        Weapon* currentWeapon;
        
        
        int iPoints = 0;
        bool pointIncreased = true; // Just nu sätts den till true för att namnet ska uppdateras första gången genom loopen.s
        
};

#endif