#ifndef LEVEL_H
#define LEVEL_H
#include "GameEngine.h"
#include <initializer_list>
#include "Sprite.h"
class Sprite;
class GameEngine;
class Level
{
    public:
        static Level* GetInstance(unsigned int lvlIndex);
        void AddSprite(Sprite* spr);
        void RemoveSprite(Sprite* s);
        void Update(GameEngine& engine);

        const std::vector<Sprite*>& GetSprites() { return sprites; }
        const unsigned int GetLevelIndex() { return levelIndex; }
    protected:
        Level(unsigned int lvlIndex);

    private:
        unsigned int levelIndex;
        std::vector<Sprite*> sprites;
        std::vector<Sprite*> added;
        std::vector<Sprite*> removed;
};


#endif