#include "GameEngine.h"
#include "MovableSprite.h"

void Attack()
{
    std::cout << "Attack\n";
}

void Move()
{
    std::cout << "Move\n";
}

#define FPS 60

int main(int argv, char **argc)
{
    MovableSprite* mvSpr = MovableSprite::GetInstance({0, 0}, {100, 100}, "MainMenuBackground.png");
    
    gameEngine.SetFps(60);

    gameEngine.RegisterKeyCallback('c', Attack);
    gameEngine.RegisterKeyCallback('a', mvSpr, &MovableSprite::Print);

    gameEngine.Run();

    return 0;
}