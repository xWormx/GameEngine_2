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
        
        void SetPosition(Vec2i p) { textFragDestRect.x = p.x; textFragDestRect.y = p.y; }
        void Blink();

        const virtual SDL_Rect& GetDestRect() {return textFragDestRect; }
        const virtual SDL_Rect& GetSrcRect() {return textFragSrcRect; }

        std::string GetText() { return strText; }
        void SetText(std::string txt);

    protected:
        TextFragment(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col, int fontIndex);
        ~TextFragment();

    private:
        SDL_Rect textFragDestRect, textFragSrcRect;
        SDL_Color color;
        SDL_Texture* textTexture = nullptr;
        int textFontIndex;
        std::string strText;

        bool blinkActivated = false, blinkOn = false;
        int blinkStep = 0;
        int numberOfBlinks = 0;
        void StartBlinking();
};


#endif