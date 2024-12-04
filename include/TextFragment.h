#ifndef TEXTFRAGMENT_H
#define TEXTFRAGMENT_H

#include "MovableSprite.h"
#include <string>

class TextFragment : public Sprite
{
    public:
        static TextFragment* GetInstance(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col);
        void Tick();
        void Draw() const;
        void OnKeyDown();

        void SetText(std::string txt);
    protected:
        TextFragment(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col);
        ~TextFragment();

    private:
        SDL_Rect textFragDestRect, textFragSrcRect;
        SDL_Color color;
        SDL_Texture* textTexture = nullptr;

};


#endif