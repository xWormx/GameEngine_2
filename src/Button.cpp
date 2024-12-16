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
    OnMouseHover();
    OnMousePress();
    OnMouseRelease();
}


Button::~Button()
{
    
}