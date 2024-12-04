#ifndef COLLIDER2D_H
#define COLLIDER2D_H

#include "SDL2\SDL.h"

class Collider2D
{
    public:
        static Collider2D* GetInstance(SDL_Rect boundsRect, bool bStatic);
        
        bool IsStatic() {return isStatic;}
        const SDL_Rect& GetBounds() {return bounds;}
        
        void SetBounds(const SDL_Rect& boundsRect) { bounds = boundsRect; }

    protected:
        Collider2D(SDL_Rect boundsRect, bool bStatic);

    private:
        SDL_Rect bounds;
        bool isStatic;
};

#endif