#include "TextFragment.h"

TextFragment* TextFragment::GetInstance(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col)
{
    return new TextFragment(pos, sz, txt, col);
}

TextFragment::TextFragment(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col) : MovableSprite(), textFragDestRect{pos.x, pos.y, sz.x, sz.y}, color(col)
{
    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(), txt.c_str(), color);
    
    if(textTexture)
        SDL_DestroyTexture(textTexture);
     
    textTexture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);

    if(!textTexture)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("TextFragment() - Couldn't CreateTextureFromSurface: " + errMsg);
    }

    textFragSrcRect.x = 0;
    textFragSrcRect.y = 0;
    SDL_QueryTexture(textTexture, 0, 0, &textFragSrcRect.w, &textFragSrcRect.h);
    SDL_QueryTexture(textTexture, 0, 0, &textFragDestRect.w, &textFragDestRect.h);

    SDL_FreeSurface(surface);   
}

void TextFragment::Draw() const
{
    SDL_RenderCopy(gameEngine.GetRenderer(), textTexture, &textFragSrcRect, &textFragDestRect);
}

void TextFragment::Tick()
{
    
}

int num = 0;
void TextFragment::OnKeyDown(const SDL_Event& e)
{
    std::string number = "num = " + std::to_string(num++);
    //SetText(number);
}

void TextFragment::SetText(std::string txt)
{
    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(), txt.c_str(), color);
    
    if(textTexture)
        SDL_DestroyTexture(textTexture);
     
    textTexture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);

    if(!textTexture)
    {
        std::string errMsg = SDL_GetError();
        throw std::runtime_error("TextFragment() - Couldn't CreateTextureFromSurface: " + errMsg);
    }

    textFragSrcRect.x = 0;
    textFragSrcRect.y = 0;
    SDL_QueryTexture(textTexture, 0, 0, &textFragSrcRect.w, &textFragSrcRect.h);
    SDL_QueryTexture(textTexture, 0, 0, &textFragDestRect.w, &textFragDestRect.h);
    
    SDL_FreeSurface(surface);   
}


void TextFragment::Move(Vec2i& movement)
{ 
    textFragDestRect.x += movement.x;
    textFragDestRect.y += movement.y;
}

TextFragment::~TextFragment()
{
    SDL_DestroyTexture(textTexture);
}