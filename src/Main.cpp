#include "GameEngine.h"

#define FPS 60

int main(int argv, char **argc)
{
    GameEngine gameEngine(600, 400, FPS);

    gameEngine.Run();

    return 0;
}