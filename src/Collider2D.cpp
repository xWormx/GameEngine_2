#include "Collider2D.h"

Collider2D* Collider2D::GetInstance(SDL_Rect boundsRect, bool bStatic)
{
    return new Collider2D(boundsRect, bStatic);
}

Collider2D::Collider2D(SDL_Rect boundsRect, bool bStatic) : bounds(boundsRect), isStatic(bStatic)
{

}