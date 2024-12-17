#ifndef TEXTFRAGMENT_H
#define TEXTFRAGMENT_H

#include "MovableSprite.h"
#include <string>

class TextFragment : public MovableSprite
{
    public:
        static TextFragment* GetInstance(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col, int fontIndex);
        void Tick();
        void Draw() const;
        void OnKeyDown(const SDL_Event& e);
        void Move(Vec2i& movement);
        
        const virtual SDL_Rect& GetDestRect() {return textFragDestRect; }
        const virtual SDL_Rect& GetSrcRect() {return textFragSrcRect; }

        void SetText(std::string txt);

    protected:
        TextFragment(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col, int fontIndex);
        ~TextFragment();

    private:
        SDL_Rect textFragDestRect, textFragSrcRect;
        SDL_Color color;
        SDL_Texture* textTexture = nullptr;

};


#endif