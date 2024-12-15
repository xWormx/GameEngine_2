#include "TextField.h"

TextField* TextField::GetInstance(Vec2i p, SDL_Color c)
{
    return new TextField(p, c);
}

TextField::TextField(Vec2i p, SDL_Color c) : MovableSprite(), textFieldDestRect({p.x, p.y, 0, 0}), color(c)
{
}

void TextField::Draw() const
{
    SDL_RenderCopy(gameEngine.GetRenderer(), textFieldTexture, &textFieldSrcRect, &textFieldDestRect);
}

void TextField::Tick()
{
    if(gameEngine.GetTextInputRecieved())
    {
        gameEngine.AppendTextInput(currentText);
        RenderNewCurrentText();
    }

    if(gameEngine.GetKeyPressedOnce(SDLK_BACKSPACE))
    {
         currentText.pop_back();
         RenderNewCurrentText();
    }   
}

void TextField::OnKeyDown(const SDL_Event& e)
{
    if(e.key.keysym.sym == SDLK_BACKSPACE)
        currentText.pop_back();
    RenderNewCurrentText();
}

void TextField::RenderNewCurrentText()
{
    // Om strängen är tom kommer den inte kunna renderas, därför måste den sättas till " "
    if(currentText.empty())
        currentText = " ";
    
    std::string errMsg;
    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(), currentText.c_str(), color);

    if(surface == nullptr)
    {
        errMsg = TTF_GetError();
        throw std::runtime_error("RenderNewCurrentText(): Coudln't RenderText Solid: " + errMsg);
    }

    textFieldDestRect.w = surface->w;
    textFieldDestRect.h = surface->h;

    if(textFieldTexture != nullptr)
        SDL_DestroyTexture(textFieldTexture);
    
    textFieldTexture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);

    if(textFieldTexture == nullptr)
    {
        errMsg = SDL_GetError();
        throw std::runtime_error("RenderNewCurrentText(): Coudln't Create Texture from surface:  " + errMsg);
    }

    SDL_Rect r = {};
    if(SDL_QueryTexture(textFieldTexture, 0, 0, &r.w, &r.h) < 0)
    {
        errMsg = SDL_GetError();
        throw std::runtime_error("RenderNewCurrentText(): Coudln't query Texture:  " + errMsg);
    }
    textFieldSrcRect.x = 0;
    textFieldSrcRect.y = 0;
    textFieldSrcRect.w = r.w;
    textFieldSrcRect.h = r.h;

    SDL_FreeSurface(surface);
}

TextField::~TextField()
{
    SDL_DestroyTexture(textFieldTexture);
}