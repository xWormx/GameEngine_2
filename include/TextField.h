#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "TextFragment.h"

class TextField : public MovableSprite
{
    public:
        static TextField* GetInstance(Vec2i p, SDL_Color c, int fontIndex);
        void Tick();
        void Draw() const;
        void OnKeyDown(const SDL_Event& e);

    protected:
        TextField(Vec2i p, SDL_Color c, int fontIndex);
        ~TextField();

    private:
        std::string currentText = " ";
        SDL_Texture* textFieldTexture = nullptr;
        SDL_Rect textFieldDestRect, textFieldSrcRect;
        SDL_Color color;
        int iFontIndex;

        void RenderNewCurrentText();

};

#endif