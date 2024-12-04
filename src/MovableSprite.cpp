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
    SDL_RenderCopy(gameEngine.GetRenderer(), movableTexture, &movableSrcRect, &movableDestRect);
}
 
void MovableSprite::Tick()
{

}

MovableSprite::~MovableSprite()
{
    SDL_DestroyTexture(movableTexture);
}