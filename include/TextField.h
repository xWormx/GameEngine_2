#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "TextFragment.h"
#include "Vec2i.h"

class TextField : public MovableSprite
{
    public:
        static TextField* GetInstance(Vec2i p, Vec2i activationSz, SDL_Color c, int fontIndex, int maxInputLength);
        void Tick();
        void Draw() const;
        virtual void OnEnterExecute(){}
        std::string GetCurrentTextInput() { return currentText; }
        const SDL_Rect& GetDestRect() { return textFieldDestRect; }
        
    protected:
        TextField(Vec2i p, Vec2i activationSz, SDL_Color c, int fontIndex, int maxInputLength);
        ~TextField();

    private:
        std::string currentText = " ", strCursor = "_";
        SDL_Texture* textFieldTexture = nullptr; 
        SDL_Texture* cursorTexture = nullptr;
        SDL_Rect textFieldDestRect, textFieldSrcRect, cursorDestRect, cursorSrcRect;
        Vec2i activationSize;
        SDL_Color color;
        int iFontIndex;
        int maxTextInputLength;
        
        bool activated = false;
        void RenderNewCurrentText();
        void ClearCursor();
        void RenderCursor();

};

#endif