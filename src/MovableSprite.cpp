#include "MovableSprite.h"

MovableSprite* MovableSprite::GetInstance(Vec2i p, Vec2i sz, std::string srcImage)
{
    return new MovableSprite(p, sz, srcImage);
}

MovableSprite::MovableSprite(Vec2i p, Vec2i sz, std::string srcImage) : Sprite(p, sz), movableDestRect{p.x, p.y, sz.x, sz.y}
{
    std::string fullSrcPath = constants::gResPath + "images\\" + srcImage;

    SDL_Surface* surface = IMG_Load(fullSrcPath.c_str());
    
    std::string errMsg = SDL_GetError();
    if(!surface)
        throw std::runtime_error("Sprite: Could't load Surface from IMG_Load(): " + errMsg);
    
    movableTexture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);
    
    if(!movableTexture)
        throw std::runtime_error("Sprite: Could't load Texture from CreateTextureFromSurface(): " + errMsg);
    
    movableSrcRect.y = 0;
    movableSrcRect.x = 0;
    SDL_QueryTexture(movableTexture, 0, 0, &movableSrcRect.w, &movableSrcRect.h);
    SDL_FreeSurface(surface);
}

void MovableSprite::Draw() const
{
    //SDL_RenderCopy(gameEngine.GetRenderer(), movableTexture, &movableSrcRect, &movableDestRect);
    SDL_RenderCopyEx(gameEngine.GetRenderer(), movableTexture, &movableSrcRect, &movableDestRect, 0.0f, NULL, SDL_FLIP_NONE);
}
 
void MovableSprite::Tick()
{

}

void MovableSprite::Move(Vec2i& movement)
{ 
    movableDestRect.x += movement.x;
    movableDestRect.y += movement.y;
    if(HasCollider2D())
    {
        SDL_Rect bounds = GetCollider2D().GetBounds();
        GetCollider2D().SetBounds({bounds.x + movement.x, bounds.y + movement.y,
                                bounds.w, bounds.h});
    } 
}

void MovableSprite::SetSpriteRegion(Vec2i p, Vec2i sz)
{
    movableSrcRect.x = p.x;
    movableSrcRect.y = p.y;
    movableSrcRect.w = sz.x;
    movableSrcRect.h = sz.y;
}

int MovableSprite::AnimateSprite(Vec2i frameStart, Vec2i frameSize, unsigned int maxFrames, unsigned int animDelay)
{
    if(animDelay == 0)
        throw std::runtime_error("The animation delay must be greater than 0!\n");

    if(frameStart.x != lastAnimationFrameStart.x || frameStart.y != lastAnimationFrameStart.y)
    {
        animationTick = 0;
        animationFrame = 0;
    }

    if(animationTick++ % animDelay == 0)
    {
        if(animationFrame >= maxFrames)
        {
            animationFrame = 0;
        }

        Vec2i p = {(frameStart.x * frameSize.x) + (animationFrame * frameSize.x), 0};
        Vec2i sz = {frameSize.x, frameSize.y};
        SetSpriteRegion(p, sz);
        animationFrame++;

    }
    lastAnimationFrameStart = frameStart;
    return animationFrame;
}

MovableSprite::~MovableSprite()
{
    SDL_DestroyTexture(movableTexture);
}