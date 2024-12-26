#include "TextFragment.h"

TextFragment* TextFragment::GetInstance(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col, int fontIndex)
{
    return new TextFragment(pos, sz, txt, col, fontIndex);
}

TextFragment::TextFragment(Vec2i pos, Vec2i sz, std::string txt, SDL_Color col, int fontIndex) : MovableSprite(), 
textFragDestRect{pos.x, pos.y, sz.x, sz.y}, color(col), textFontIndex(fontIndex), strText(txt)
{
    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(fontIndex), txt.c_str(), color);
    
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
    if(blinkActivated)
        StartBlinking();
}

void TextFragment::Blink()
{
    blinkActivated = true;
    blinkOn = true;
}

void TextFragment::StartBlinking()
{
    if(numberOfBlinks < 10)
    {
        if(blinkStep++ % 3 == 0)
        {
            if(blinkOn)
            {
                color = {255,255,255,255};
                blinkOn = false;
            }
            else
            {
                color = {0,255,0,255};
                blinkOn = true;
            }
            numberOfBlinks++;
        }
    }
    else
    {
        blinkActivated = false;
        blinkOn = false;
        color = {0,255,0,255};
        numberOfBlinks = 0;                
    }

    SetText(strText);
}

void TextFragment::SetText(std::string txt)
{
    strText = txt;
    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(textFontIndex), txt.c_str(), color);
    
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