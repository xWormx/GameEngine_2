#ifndef SPRITE_H
#define SPRITE_H

#include "Vec2i.h"

class Sprite
{
    virtual void Draw() const = 0;
    virtual void TIck() = 0;
    virtual void OnKeyDown() {}
    virtual void OnKeyUp() {}
    virtual void OnMouseUp() {}
    virtual void OnMouseDown() {}
    virtual ~Sprite();

    protected:
        Sprite(Vec2i pos, Vec2i size);

};


#endif