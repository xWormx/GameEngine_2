#include "Player.h"

Player::Player(Vec2i p, Vec2i sz, std::string srcImage, TextFragment* tfName, TextFragment* _tfScore, int health) : MovableSprite(p, sz, srcImage), 
                name(tfName), tfScore(_tfScore)
{
    healthBar = new HealthBar({p.x - 30, p.y - 30}, {100, 10}, "HealthBar.png", health); 
    healthBar->SetColor({1, 255, 1, 255});
    gameEngine.GetLevelAtIndex(1)->AddSprite(healthBar);
    name->SetPosition({healthBar->GetDestRect().x, healthBar->GetDestRect().y - name->GetDestRect().h});


}

void Player::Tick()
{
    if(pointIncreased)
    {
        UpdatePoints();
    }

    static int s = 3;
    if(gameEngine.GetKeyPressed(rightKey))
        speed.x = s; 
    if(gameEngine.GetKeyPressed(leftKey))
        speed.x = -s;
    if(gameEngine.GetKeyPressed(upKey))
        speed.y = -s;
    if(gameEngine.GetKeyPressed(downKey))
        speed.y = s;
    if(speed.x != 0 || speed.y != 0)
    {
        Move(speed);
        name->Move(speed);
        healthBar->MoveBar(speed);
        speed = {0,0};
    }
    
    AnimateSprite({0, 0}, {32, 80}, 4, 5);

    if(gameEngine.GetKeyPressedOnce('p'))
    {
        if(GetNameTag() == "player1")
        {
            currentWeapon->Shoot();
        }
    }

    if(gameEngine.GetKeyPressedOnce('b'))
    {
        if(GetNameTag() == "player2")
        {
            SDL_Rect destRect = GetDestRect();
            Bullet* p = new Bullet({destRect.x + destRect.w, destRect.y + (destRect.h / 2)}, {32,16}, "BulletTest.png", this);
            p->SetTag("WeaponShot");
            SDL_Rect bounds = p->GetDestRect();
            p->InstallCollider2D(bounds, false);
            gameEngine.GetCurrentLevel()->AddSprite(p);
            gameEngine.PlaySound("shot", 3);
        }
    }
}