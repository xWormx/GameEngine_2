#include "Weapon.h"

Weapon::Weapon(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner) : MovableSprite(p, sz, srcImage), owner(_owner)
{}


void Weapon::Tick()
{
    Vec2i p = {owner->GetDestRect().x, owner->GetDestRect().y + 20};
    SetPosition(p);
}

void Weapon::Shoot()
{
    SDL_Rect destRect = GetDestRect();
    Bullet* p = new Bullet({destRect.x + destRect.w, destRect.y + (destRect.h / 2)}, {32,16}, "BulletTest.png", owner);
    p->SetTag("BulletShot");
    SDL_Rect bounds = p->GetDestRect();
    p->InstallCollider2D(bounds, false);
    gameEngine.GetCurrentLevel()->AddSprite(p);
    gameEngine.PlaySound("shot", 3);
}
