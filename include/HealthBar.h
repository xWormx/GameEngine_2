#ifndef HEALTHBAR_H
#define HEALTHBAR_H


#include "MovableSprite.h"

class HealthBar : public MovableSprite
{
    public:
        HealthBar(Vec2i p, Vec2i sz, std::string srcImage, int maxHP) : MovableSprite(p, sz, srcImage),
                    maxHealth(maxHP), currentHealth(maxHP)
        {
            background = MovableSprite::GetInstance(p, sz, srcImage);
            gameEngine.GetLevelAtIndex(1)->AddSprite(background);
        }

        void ApplyDamage(int damageAmount) 
        { 
            currentHealth -= damageAmount; 
            if(currentHealth < 0)
                currentHealth = 0;
            
            double percentageToAdjust = (double)currentHealth / (double)maxHealth;
            // Multiplicera med background för att den inte ändras.
            double adjustWidth = (double)background->GetDestRect().w * percentageToAdjust;
            SetSize({(int)adjustWidth, background->GetDestRect().h});
            
        }

        void ApplyHealing(int healingAmount) 
        { 
            if(currentHealth < maxHealth)
                currentHealth += healingAmount; 
            if(currentHealth > maxHealth)
                currentHealth = maxHealth;
        }

        void MoveBar(Vec2i speed)
        {
            Move(speed);
            background->Move(speed);
        }

        void DeleteBar()
        {
            gameEngine.GetCurrentLevel()->RemoveSprite(background);
            gameEngine.GetCurrentLevel()->RemoveSprite(this);
        }

        int GetCurrentHealth() { return currentHealth; }

        ~HealthBar()
        {           
        }

    private:
        int maxHealth, currentHealth;
        MovableSprite* background;
};

#endif