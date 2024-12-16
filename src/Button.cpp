#include "Button.h"

Button* Button::GetInstance(Vec2i p, Vec2i sz, std::string srcImage)
{
    return new Button(p, sz, srcImage);
}

Button::Button(Vec2i p, Vec2i sz, std::string srcImage) : MovableSprite(p, sz, srcImage)
{
    
}

void Button::Tick()
{
    Vec2i mp = gameEngine.GetMousePosition();
    if((mp.x > GetDestRect().x  &&  mp.x < GetDestRect().x + GetDestRect().w) &&
        mp.y > GetDestRect().y && mp.y < GetDestRect().y + GetDestRect().h)
    {
        mouseHover = true;
        OnMouseEnter();
    }
    else
    {
        mouseHover = false;
        OnMouseExit();
    }

    if(gameEngine.GetMousePressed(SDL_BUTTON_LEFT))
    {  
        if(mouseHover)
        {
            OnMousePress();   
            mousePress = true; 
        }
    }
    else
    {
        if(mousePress && mouseHover)
        {
            mousePress = false;
            OnMouseRelease();
        }
        else
        {
            mousePress = false;
        }
    }

        

}


Button::~Button()
{
    
}