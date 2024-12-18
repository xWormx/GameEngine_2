#include "Level.h"

Level* Level::GetInstance(unsigned int lvlIndex)
{
    return new Level(lvlIndex);    
}   

Level::Level(unsigned int lvlIndex) : levelIndex(lvlIndex)
{
}

void Level::AddSprite(Sprite* s)
{
    if(!s)
        throw std::runtime_error("AddSprite: Sprite was not valid");
    
    for(Sprite * existingSprte : sprites)
        if(s == existingSprte)
            throw std::runtime_error("AddSprite: Sprite already exists!");
    
    added.push_back(s);

    if(s->HasCollider2D())
        colliderSprites.push_back(s);
}

void Level::RemoveSprite(Sprite* s)
{
    for(Sprite* existingSprite : removed)
        if(existingSprite == s) 
            return;
        
    removed.push_back(s);
}

void Level::Update(GameEngine& engine)
{
    for(Sprite* s : sprites)
    {
        s->Tick();
        /* s->UpdateRgbd()
        {
            pos = gravity;
        } */
    }
        
/* 
    Loop rgbd */

/* 
    Loop collisions
    {

    }
 */
    for(Sprite* s : added)
        sprites.push_back(s);

    added.clear();

    for(Sprite* s : removed)
    {
        for(std::vector<Sprite*>::iterator i = sprites.begin(); i != sprites.end();)
        {
            if(*i == s)
            {
                i = sprites.erase(i);
            }
            else
                i++;
        }

        for(std::vector<Sprite*>::iterator ci = colliderSprites.begin(); ci != colliderSprites.end();)
        {
            if(*ci == s)
                ci = colliderSprites.erase(ci);
            else
                ci++;
        }

        delete s;
    }

    removed.clear();
}