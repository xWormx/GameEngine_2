#ifndef BUTTON_H
#define BUTTON_H

#include "MovableSprite.h"

class Button : public MovableSprite
{
    public:
        static Button* GetInstance(Vec2i p, Vec2i sz, std::string srcImage);
        void Tick();
        virtual void OnMouseHover(){}
        virtual void OnMousePress(){}
        virtual void OnMouseRelease(){}
        ~Button();

    protected:
        Button(Vec2i p, Vec2i sz, std::string srcImage);

    private:
        
};


#endif