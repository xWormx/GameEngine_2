#include "TextField.h"

TextField* TextField::GetInstance(Vec2i p, Vec2i activationSz, SDL_Color c, int fontIndex, int maxInputLength)
{
    return new TextField(p, activationSz, c, fontIndex, maxInputLength);
}

TextField::TextField(Vec2i p, Vec2i activationSz, SDL_Color c, int fontIndex, int maxInputLength) : MovableSprite(), 
                    textFieldDestRect({p.x + 18, p.y, 0, 0}), activationSize(activationSz), color(c), iFontIndex(fontIndex), maxTextInputLength(maxInputLength)
{
}

void TextField::Draw() const
{
    SDL_RenderCopy(gameEngine.GetRenderer(), textFieldTexture, &textFieldSrcRect, &textFieldDestRect);
    SDL_RenderCopy(gameEngine.GetRenderer(), cursorTexture, &cursorSrcRect, &cursorDestRect);
}


void TextField::Tick()
{
    Vec2i mp = gameEngine.GetMousePosition();
    if((mp.x > GetDestRect().x  &&  mp.x < GetDestRect().x + activationSize.x) &&
        mp.y > GetDestRect().y && mp.y < GetDestRect().y + activationSize.y)
    {
        if(gameEngine.GetMousePressed(SDL_BUTTON_LEFT))
            activated = true;
    }
    else
    {
        if(gameEngine.GetMousePressed(SDL_BUTTON_LEFT))
        {
            activated = false;
            ClearCursor();
        }
            
    }

    if(activated)
    {       
        if(gameEngine.GetTextInputRecieved())
        {
            if(currentText.size() <= maxTextInputLength)
            {
                if(currentText == " ")
                    currentText.pop_back();
                
                gameEngine.AppendTextInput(currentText);
                RenderNewCurrentText();
            }
        }

        if(gameEngine.GetKeyPressedOnce(SDLK_BACKSPACE))
        {
            currentText.pop_back();            
            RenderNewCurrentText();
        }

        if(gameEngine.GetKeyPressedOnce(SDLK_RETURN))
        {
            OnEnterExecute();
            currentText.clear();
            currentText = " ";
            RenderNewCurrentText();
        }

        static int cursorTick = 0;
        if(cursorTick++ % 10 == 0)
            RenderCursor();
    }  

    cursorDestRect.y = textFieldDestRect.y;       
    if(currentText != " ")
        cursorDestRect.x = textFieldDestRect.x + textFieldDestRect.w;
    else    
        cursorDestRect.x = textFieldDestRect.x;
}

void TextField::RenderNewCurrentText()
{
    // Om strängen är tom kommer den inte kunna renderas, därför måste den sättas till " "
    if(currentText.empty())
        currentText = " ";
    
    std::string errMsg;
    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(iFontIndex), currentText.c_str(), color);

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

void TextField::ClearCursor()
{
    strCursor = " ";

    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(iFontIndex), strCursor.c_str(), color);
    if(!surface)
        throw std::runtime_error("Couldn't load surface for cursor in textfield");


    cursorDestRect.w = surface->w;
    cursorDestRect.h = surface->h;
    
    if(cursorTexture != nullptr)
        SDL_DestroyTexture(cursorTexture);

    cursorTexture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);

    if(!cursorTexture)
        throw std::runtime_error("Couldn't load texture for cursor in textfield");
    
    int srcW, srcH;
    if(SDL_QueryTexture(cursorTexture, 0, 0, &srcW, &srcH) < 0)
        throw std::runtime_error("Couldn't query texture for cursor in textfield");
        
    cursorSrcRect.x = 0;
    cursorSrcRect.y = 0;
    cursorSrcRect.w = srcW;
    cursorSrcRect.h = srcH;

    SDL_FreeSurface(surface);
}

void TextField::RenderCursor()
{
    if(strCursor == "_")
        strCursor = " ";
    else
        strCursor = "_";

    SDL_Surface* surface = TTF_RenderText_Solid(gameEngine.GetFont(iFontIndex), strCursor.c_str(), color);
    if(!surface)
        throw std::runtime_error("Couldn't load surface for cursor in textfield");

    cursorDestRect.w = surface->w;
    cursorDestRect.h = surface->h;
    
    if(cursorTexture != nullptr)
        SDL_DestroyTexture(cursorTexture);

    cursorTexture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);

    if(!cursorTexture)
        throw std::runtime_error("Couldn't load texture for cursor in textfield");
    
    int srcW, srcH;
    if(SDL_QueryTexture(cursorTexture, 0, 0, &srcW, &srcH) < 0)
        throw std::runtime_error("Couldn't query texture for cursor in textfield");
        
    cursorSrcRect.x = 0;
    cursorSrcRect.y = 0;
    cursorSrcRect.w = srcW;
    cursorSrcRect.h = srcH;

    SDL_FreeSurface(surface);
}

TextField::~TextField()
{
    SDL_DestroyTexture(textFieldTexture);
    SDL_DestroyTexture(cursorTexture);
}