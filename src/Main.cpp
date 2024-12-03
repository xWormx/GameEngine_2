#include "GameEngine.h"
#include "MovableSprite.h"
#include "Level.h"

void Attack()
{
    std::cout << "Attack\n";
}

void Move()
{
    std::cout << "Move\n";
}

#define FPS 60


static int x = 0;
static int y = 0;
    
Level* level1 = Level::GetInstance(0);

void AddImageToLevel()
{
    MovableSprite* mvSpr = MovableSprite::GetInstance({x * 100, y * 20}, {100, 100}, "MainMenuBackground.png");
    x++;
    if(x >= 5)
    {
        y++;
        x = 0;
    }
        
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
    
    MovableSprite* mvSpr = MovableSprite::GetInstance({0, 0}, {100, 100}, "MainMenuBackground.png");
    

    level1->AddSprite(mvSpr);
    gameEngine.SetFps(60);

    gameEngine.AddLevel(level1);
    gameEngine.RegisterKeyCallback('c', Attack);
    gameEngine.RegisterKeyCallback('c', AddImageToLevel);
    gameEngine.RegisterKeyCallback('d', RemoveImageFromLevel);
    gameEngine.RegisterKeyCallback('a', mvSpr, &MovableSprite::Print);

    gameEngine.Run();

    return 0;
}