#include "Bullet.h"

Bullet::Bullet(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner) : MovableSprite(p, sz, srcImage), owner(_owner)
{
    velocity.x = 13; //gameEngine.GetRandomNumberInRange(3, 8);
    velocity.y = gameEngine.GetRandomNumberInRange(-2, 2);
}

void Bullet::Tick()
{
    if (lifeTime-- < 0)
    {
        SpawnExplosion();
        gameEngine.GetCurrentLevel()->RemoveSprite(this);
    }
    else
    {
        Move(velocity);
    }
}

/*    cos(0)      cos(90)
    x   10  6   3   0   -3  -6  -10    
    y

        1   2   3   4   5   6   7   8   9   10    11     12  13   14    15


                |

        -               - sin(0)  = 0, cos(0) = 1


                |



*/

void Bullet::SpawnExplosion()
{
    for (int i = 0; i < 15; i++)
    {
        Explosion* p = new Explosion({ GetDestRect().x, GetDestRect().y }, { 16, 16 }, "Particle.png", owner);
        int posY = (int)(10 * std::sin(i * 18));
        int posX = (int)(10 * std::cos(i * 18));
        p->SetVelocity({posX, posY}); 
        p->SetLifeTime(15);
        //p->SetVelocity({gameEngine.GetRandomNumberInRange(-5, 8), gameEngine.GetRandomNumberInRange(-5, 5)}); 
        //p->SetLifeTime(gameEngine.GetRandomNumberInRange(5, 35));
        p->SetTag("explosion");  // This works now because Explosion is fully defined
        p->InstallCollider2D(p->GetDestRect(), false);  // This works too
        gameEngine.GetCurrentLevel()->AddSprite(p);
        gameEngine.PlaySound("release", 1);
    }
}


void Bullet::OnCollision2D(Sprite* other)
{
    if (other->GetNameTag() == "enemy")
        lifeTime = -1;
}
