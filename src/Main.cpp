#include "GameEngine.h"
#include "MovableSprite.h"
#include "Level.h"
#include "TextFragment.h"

void Attack()
{
    std::cout << "Attack\n";
}

void Move()
{
    std::cout << "Move\n";
}

#define FPS 60


static int x = 1;
static int y = 0;
    
Level* level1 = Level::GetInstance(0);

void AddImageToLevel()
{
    MovableSprite* mvSpr = MovableSprite::GetInstance({x * 100, y * 20}, {64, 64}, "MainMenuBackground.png");
    x++;
    if(x >= 5)
    {
        y++;
        x = 0;
    }
    const SDL_Rect& r = mvSpr->GetDestRect();
    mvSpr->InstallCollider2D({r.x, r.y, r.w, r.h}, false);   
    level1->AddSprite(mvSpr);
}
void RemoveImageFromLevel()
{
    std::vector<Sprite*> s = level1->GetSprites();
    level1->RemoveSprite(s.back());
    x--;
    if(x < 0)
    {
        y--;
        x = 5;
    }       
}

int main(int argv, char **argc)
{
    MovableSprite* mvSpr = MovableSprite::GetInstance({0, 0}, {64, 64}, "MainMenuBackground.png");
    
    /* mvSpr->InstallCollider2D();
    mvSpr->InstallRigidbody2D();
    mvSpr->SetGravity(9.8);
    mvSpr->SetElasticity(2);
 */

    TextFragment* text1 = TextFragment::GetInstance({100, 100}, {100, 100}, "YOOOO", {255, 0, 0, 255});
    level1->AddSprite(mvSpr);
    level1->AddSprite(text1);
    
    gameEngine.SetFps(60);

    gameEngine.AddLevel(level1);
    gameEngine.RegisterKeyCallback('c', Attack);
    gameEngine.RegisterKeyCallback('c', AddImageToLevel);
    gameEngine.RegisterKeyCallback('d', RemoveImageFromLevel);
    gameEngine.RegisterKeyCallback('a', mvSpr, &MovableSprite::Print);



    gameEngine.Run();

    return 0;
}