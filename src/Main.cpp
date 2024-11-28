#include "GameEngine.h"

int main(int argv, char **argc)
{
    GameEngine gameEngine(600, 400, 60);

    gameEngine.Run();

    return 0;
}